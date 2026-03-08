#ifndef CJSON_H
#define CJSON_H

// cJSON 的基本类型定义
typedef enum {
    cJSON_NULL,
    cJSON_False,
    cJSON_True,
    cJSON_Number,
    cJSON_String,
    cJSON_Array,
    cJSON_Object
} cJSON_Type;

// cJSON 结构体
typedef struct cJSON {
    struct cJSON *next;
    struct cJSON *prev;
    struct cJSON *child;
    cJSON_Type type;
    char *valuestring;
    int valueint;
    double valuedouble;
    char *string;
} cJSON;

// 基本操作函数
cJSON *cJSON_CreateObject(void);
cJSON *cJSON_CreateArray(void);
cJSON *cJSON_CreateString(const char *string);
cJSON *cJSON_CreateNumber(double num);
cJSON *cJSON_CreateBool(int b);
cJSON *cJSON_CreateNull(void);

void cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item);
void cJSON_AddItemToArray(cJSON *array, cJSON *item);

char *cJSON_Print(cJSON *item);
char *cJSON_PrintFormatted(cJSON *item);  // 美化输出函数

void cJSON_Delete(cJSON *c);

#endif
