#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

// 测试基本的JSON创建和打印
void test_basic()
{
    printf("=== 测试1：基本JSON创建和打印 ===\n");
    
    // 创建一个简单的JSON对象
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", "张三");
    cJSON_AddNumberToObject(json, "age", 20);
    cJSON_AddBoolToObject(json, "student", 1);
    
    // 使用默认格式化打印（制表符缩进）
    char *str1 = cJSON_Print(json);
    printf("默认格式化（制表符）：\n%s\n", str1);
    free(str1);
    
    // 使用美化打印（2个空格缩进）
    char *str2 = cJSON_PrintPretty(json, 2);
    printf("美化打印（2空格）：\n%s\n", str2);
    free(str2);
    
    // 使用美化打印（4个空格缩进）
    char *str3 = cJSON_PrintPretty(json, 4);
    printf("美化打印（4空格）：\n%s\n", str3);
    free(str3);
    
    cJSON_Delete(json);
    printf("\n");
}

// 测试嵌套对象
void test_nested()
{
    printf("=== 测试2：嵌套对象 ===\n");
    
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", "李四");
    
    // 创建一个嵌套对象
    cJSON *address = cJSON_CreateObject();
    cJSON_AddStringToObject(address, "city", "北京");
    cJSON_AddStringToObject(address, "street", "中关村大街");
    cJSON_AddItemToObject(json, "address", address);
    
    // 使用2个空格缩进
    char *str = cJSON_PrintPretty(json, 2);
    printf("嵌套对象（2空格缩进）：\n%s\n", str);
    free(str);
    
    cJSON_Delete(json);
    printf("\n");
}

// 测试数组
void test_array()
{
    printf("=== 测试3：数组 ===\n");
    
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", "王五");
    
    // 创建一个数组
    cJSON *hobbies = cJSON_CreateArray();
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("读书"));
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("编程"));
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("运动"));
    cJSON_AddItemToObject(json, "hobbies", hobbies);
    
    // 使用4个空格缩进
    char *str = cJSON_PrintPretty(json, 4);
    printf("包含数组的对象（4空格缩进）：\n%s\n", str);
    free(str);
    
    cJSON_Delete(json);
    printf("\n");
}

// 测试复杂嵌套
void test_complex()
{
    printf("=== 测试4：复杂嵌套结构 ===\n");
    
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", "赵六");
    cJSON_AddNumberToObject(json, "age", 22);
    
    // 创建嵌套对象
    cJSON *info = cJSON_CreateObject();
    cJSON_AddStringToObject(info, "school", "北京大学");
    cJSON_AddStringToObject(info, "major", "计算机科学");
    
    // 创建嵌套数组
    cJSON *courses = cJSON_CreateArray();
    cJSON_AddItemToArray(courses, cJSON_CreateString("数据结构"));
    cJSON_AddItemToArray(courses, cJSON_CreateString("算法"));
    cJSON_AddItemToObject(info, "courses", courses);
    
    cJSON_AddItemToObject(json, "info", info);
    
    // 比较不同缩进的效果
    printf("使用2个空格缩进：\n");
    char *str1 = cJSON_PrintPretty(json, 2);
    printf("%s\n", str1);
    free(str1);
    
    printf("使用4个空格缩进：\n");
    char *str2 = cJSON_PrintPretty(json, 4);
    printf("%s\n", str2);
    free(str2);
    
    cJSON_Delete(json);
    printf("\n");
}

// 测试解析和重新格式化
void test_parse_and_format()
{
    printf("=== 测试5：解析JSON并重新格式化 ===\n");
    
    // 一个紧凑格式的JSON字符串
    const char *json_str = "{\"name\":\"测试\",\"age\":18,\"scores\":[95,87,92]}";
    
    // 解析JSON
    cJSON *json = cJSON_Parse(json_str);
    if (json == NULL)
    {
        printf("解析失败！\n");
        return;
    }
    
    printf("原始紧凑格式：\n%s\n\n", json_str);
    
    // 使用美化打印
    char *pretty = cJSON_PrintPretty(json, 2);
    printf("美化后（2空格）：\n%s\n", pretty);
    free(pretty);
    
    cJSON_Delete(json);
    printf("\n");
}

int main()
{
    printf("cJSON 格式化美化功能测试\n");
    printf("========================\n\n");
    
    test_basic();
    test_nested();
    test_array();
    test_complex();
    test_parse_and_format();
    
    printf("所有测试完成！\n");
    return 0;
}
