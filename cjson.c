// 第三周
#include "cjson.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 创建一个新的 cJSON 对象
cJSON *cJSON_CreateObject(void) {
    cJSON *item = (cJSON *)malloc(sizeof(cJSON));
    if (item) {
        memset(item, 0, sizeof(cJSON));
        item->type = cJSON_Object;
    }
    return item;
}

// 创建一个数组
cJSON *cJSON_CreateArray(void) {
    cJSON *item = (cJSON *)malloc(sizeof(cJSON));
    if (item) {
        memset(item, 0, sizeof(cJSON));
        item->type = cJSON_Array;
    }
    return item;
}

// 创建字符串
cJSON *cJSON_CreateString(const char *string) {
    cJSON *item = (cJSON *)malloc(sizeof(cJSON));
    if (item) {
        memset(item, 0, sizeof(cJSON));
        item->type = cJSON_String;
        item->valuestring = strdup(string);
    }
    return item;
}

// 创建数字
cJSON *cJSON_CreateNumber(double num) {
    cJSON *item = (cJSON *)malloc(sizeof(cJSON));
    if (item) {
        memset(item, 0, sizeof(cJSON));
        item->type = cJSON_Number;
        item->valuedouble = num;
        item->valueint = (int)num;
    }
    return item;
}

// 创建布尔值
cJSON *cJSON_CreateBool(int b) {
    cJSON *item = (cJSON *)malloc(sizeof(cJSON));
    if (item) {
        memset(item, 0, sizeof(cJSON));
        item->type = b ? cJSON_True : cJSON_False;
    }
    return item;
}

// 创建 NULL
cJSON *cJSON_CreateNull(void) {
    cJSON *item = (cJSON *)malloc(sizeof(cJSON));
    if (item) {
        memset(item, 0, sizeof(cJSON));
        item->type = cJSON_NULL;
    }
    return item;
}

// 向对象添加项
void cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item) {
    if (!object || !item) return;
    item->string = strdup(string);
    if (!object->child) {
        object->child = item;
    } else {
        cJSON *child = object->child;
        while (child->next) child = child->next;
        child->next = item;
        item->prev = child;
    }
}

// 向数组添加项
void cJSON_AddItemToArray(cJSON *array, cJSON *item) {
    if (!array || !item) return;
    if (!array->child) {
        array->child = item;
    } else {
        cJSON *child = array->child;
        while (child->next) child = child->next;
        child->next = item;
        item->prev = child;
    }
}

// 简单的打印函数（不美化）
char *cJSON_Print(cJSON *item) {
    // 简化实现，实际项目中会更复杂
    char *result = (char *)malloc(1024);
    sprintf(result, "{}");
    return result;
}

// 这个函数用来添加缩进，让输出更整齐
// depth 表示当前是第几层，层数越深缩进越多
static void addIndent(char **buffer, int *len, int *capacity, int depth) {
    int indentSize = depth * 2;  // 每层缩进2个空格，这样看起来更清楚
    if (*len + indentSize + 1 > *capacity) {
        *capacity = (*capacity + indentSize + 1) * 2;
        *buffer = (char *)realloc(*buffer, *capacity);
    }
    for (int i = 0; i < indentSize; i++) {
        (*buffer)[(*len)++] = ' ';
    }
}

// 这个函数用来把字符串添加到缓冲区里
// 如果缓冲区不够大，会自动扩大
static void addString(char **buffer, int *len, int *capacity, const char *str) {
    int strLen = strlen(str);
    if (*len + strLen + 1 > *capacity) {
        *capacity = (*len + strLen + 1) * 2;
        *buffer = (char *)realloc(*buffer, *capacity);
    }
    strcpy(*buffer + *len, str);
    *len += strLen;
}

// 这个函数是核心，用来格式化一个 JSON 项
// 根据不同的类型（数字、字符串、数组、对象等）进行不同的处理
static void formatItem(cJSON *item, char **buffer, int *len, int *capacity, int depth) {
    if (!item) return;
    
    switch (item->type) {
        case cJSON_NULL:
            addString(buffer, len, capacity, "null");
            break;
        case cJSON_False:
            addString(buffer, len, capacity, "false");
            break;
        case cJSON_True:
            addString(buffer, len, capacity, "true");
            break;
        case cJSON_Number:
            {
                // 处理数字，如果是整数就显示整数，否则显示小数
                char numStr[64];
                if (item->valuedouble == (double)item->valueint) {
                    sprintf(numStr, "%d", item->valueint);
                } else {
                    sprintf(numStr, "%.6g", item->valuedouble);
                }
                addString(buffer, len, capacity, numStr);
            }
            break;
        case cJSON_String:
            {
                // 处理字符串，需要加上引号，还要处理特殊字符
                char *str = (char *)malloc(strlen(item->valuestring) * 2 + 3);
                str[0] = '"';
                int pos = 1;
                // 如果字符串里有引号或反斜杠，需要转义
                for (int i = 0; item->valuestring[i]; i++) {
                    if (item->valuestring[i] == '"' || item->valuestring[i] == '\\') {
                        str[pos++] = '\\';  // 先加一个反斜杠
                    }
                    str[pos++] = item->valuestring[i];
                }
                str[pos++] = '"';
                str[pos] = '\0';
                addString(buffer, len, capacity, str);
                free(str);
            }
            break;
        case cJSON_Array:
            // 处理数组，每个元素换行显示
            addString(buffer, len, capacity, "[\n");
            {
                cJSON *child = item->child;
                int first = 1;  // 用来判断是不是第一个元素
                while (child) {
                    if (!first) {
                        addString(buffer, len, capacity, ",\n");  // 不是第一个就加逗号
                    }
                    first = 0;
                    addIndent(buffer, len, capacity, depth + 1);  // 增加一层缩进
                    formatItem(child, buffer, len, capacity, depth + 1);  // 递归处理子元素
                    child = child->next;
                }
            }
            addString(buffer, len, capacity, "\n");
            addIndent(buffer, len, capacity, depth);
            addString(buffer, len, capacity, "]");
            break;
        case cJSON_Object:
            // 处理对象，每个键值对换行显示
            addString(buffer, len, capacity, "{\n");
            {
                cJSON *child = item->child;
                int first = 1;
                while (child) {
                    if (!first) {
                        addString(buffer, len, capacity, ",\n");
                    }
                    first = 0;
                    addIndent(buffer, len, capacity, depth + 1);
                    // 先打印键名（用引号包起来）
                    addString(buffer, len, capacity, "\"");
                    addString(buffer, len, capacity, child->string ? child->string : "");
                    addString(buffer, len, capacity, "\": ");
                    // 再打印值（递归处理）
                    formatItem(child, buffer, len, capacity, depth + 1);
                    child = child->next;
                }
            }
            addString(buffer, len, capacity, "\n");
            addIndent(buffer, len, capacity, depth);
            addString(buffer, len, capacity, "}");
            break;
    }
}

// 这是新加的美化输出函数，主要功能就是把 JSON 格式化得更好看
// 通过添加缩进和换行，让 JSON 结构更清晰
char *cJSON_PrintFormatted(cJSON *item) {
    if (!item) return NULL;
    
    // 先分配一块内存用来存放格式化后的字符串
    int capacity = 1024;  // 初始大小
    int len = 0;  // 当前用了多少
    char *buffer = (char *)malloc(capacity);
    buffer[0] = '\0';
    
    // 调用格式化函数，从第0层（最外层）开始
    formatItem(item, &buffer, &len, &capacity, 0);
    
    // 确保字符串结尾有 \0，这样 printf 才能正常输出
    if (len >= capacity) {
        buffer = (char *)realloc(buffer, len + 1);
    }
    buffer[len] = '\0';
    
    return buffer;
}

// 删除 cJSON 对象
void cJSON_Delete(cJSON *c) {
    if (!c) return;
    
    // 递归删除子节点
    if (c->child) {
        cJSON_Delete(c->child);
    }
    
    // 删除兄弟节点
    if (c->next) {
        cJSON_Delete(c->next);
    }
    
    // 释放字符串内存
    if (c->valuestring) free(c->valuestring);
    if (c->string) free(c->string);
    
    // 释放自身
    free(c);
}
