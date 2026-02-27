/*
  Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/与系统头文件中的单行注释有关

/* cJSON */
/* JSON parser in C. */
//说明了该文件的名及其功能

/* disable warnings about old C89 functions in MSVC */
#if !defined(_CRT_SECURE_NO_DEPRECATE) && defined(_MSC_VER)
#define _CRT_SECURE_NO_DEPRECATE
#endif
//用 Microsoft Visual C++ (MSVC) 编译器时，禁用关于旧 C89 函数的警告

#ifdef __GNUC__
#pragma GCC visibility push(default)
#endif
//默认为 default，意味着所有符号都是可见的

//针对 MSVC 编译器
#if defined(_MSC_VER)    
#pragma warning (push)        //用于保存当前的警告状态 
/* disable warning about single line comments in system headers */
#pragma warning (disable : 4001)   //禁用特定的警告（4001),与系统头文件中的单行注释有关
#endif
//引入了字符串操作,标准输入,数学函数标准库,函数极限值字符类型检查和浮点数操作
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <float.h>

#ifdef ENABLE_LOCALES
#include <locale.h>
#endif
//如果定义了 ENABLE_LOCALES，则包含 <locale.h> 头文件。<locale.h> 用于处理本地化相关的功能，如字符分类和字符串转换
//ENABLE_LOCALES 是一个编译时宏定义，用于控制程序是否启用本地化（locale）支持

#if defined(_MSC_VER)
#pragma warning (pop)
#endif
//恢复之前保存的警告状态由 pragma warning (push )保存

#ifdef __GNUC__
#pragma GCC visibility pop
#endif
//在GNU C编译器中使用#pragma GCC visibility pop来恢复默认的符号可见性

#include "cJSON.h"
//包含"cJSON.h"头文件
//这是cJSON库的主头文件

/* define our own boolean type */
//定义自己的布尔类型
#ifdef true
#undef true
#endif
//如果已经定义了true，则取消其定义
#define true ((cJSON_bool)1)
//重新定义true为((cJSON_bool)1)，即布尔值true

#ifdef false
#undef false
#endif
#define false ((cJSON_bool)0)
//如果已经定义了false，则取消其定义

/* define isnan and isinf for ANSI C, if in C99 or above, isnan and isinf has been defined in math.h */
//如果在 ANSI C 中没有定义 isnan 和 isinf，则定义它们
#ifndef isinf            //定义isinf函数，检查一个数是否为无穷大
#define isinf(d) (isnan((d - d)) && !isnan(d))
#endif
//定义isnan函数，检查一个数是否为非数字
#ifndef isnan
#define isnan(d) (d != d)
#endif

//定义NAN（Not a Number）常量，表示非数字的值
#ifndef NAN
//在 Windows 平台上，使用 sqrt(-1.0) 一个 NaN 值
//因为在 C89 标准中没有直接定义 NAN 常量
#ifdef _WIN32
#define NAN sqrt(-1.0)
#else
//在其他平台上，使用 0.0/0.0 来生成一个 NaN 值
#define NAN 0.0/0.0
#endif
#endif

//定义一个结构体 error，用于存储解析错误的信息，包括 JSON 字符串和错误位置
typedef struct {
    const unsigned char *json;  //指向 JSON 字符串的指针
    size_t position;     //错误位置的索引
} error;     

//定义一个全局变量 global_error，用于存储当前的解析错误信息
static error global_error = { NULL, 0 };

//提供一个函数 cJSON_GetErrorPtr，用于获取当前解析错误的位置指针
CJSON_PUBLIC(const char *) cJSON_GetErrorPtr(void)
{
    return (const char*) (global_error.json + global_error.position);  //返回指向错误位置的指针
}

//定义一个函数 cJSON_SetErrorPtr，用于设置当前解析错误的位置
CJSON_PUBLIC(char *) cJSON_GetStringValue(const cJSON * const item)
{
    //如果 item 不是一个字符串类型，则返回 NULL
    if (!cJSON_IsString(item))
    {
        return NULL;
    }
//返回 item 的 valuestring 字段，即字符串的值   
    return item->valuestring;
}

CJSON_PUBLIC(double) cJSON_GetNumberValue(const cJSON * const item)
{
    if (!cJSON_IsNumber(item))
    {
        return (double) NAN;
    }

    return item->valuedouble;
}

/* This is a safeguard to prevent copy-pasters from using incompatible C and header files */
#if (CJSON_VERSION_MAJOR != 1) || (CJSON_VERSION_MINOR != 7) || (CJSON_VERSION_PATCH != 19)
    #error cJSON.h and cJSON.c have different versions. Make sure that both have the same.
#endif

CJSON_PUBLIC(const char*) cJSON_Version(void)
{
    static char version[15];
    sprintf(version, "%i.%i.%i", CJSON_VERSION_MAJOR, CJSON_VERSION_MINOR, CJSON_VERSION_PATCH);

    return version;
}

/* Case insensitive string comparison, doesn't consider two NULL pointers equal though */
static int case_insensitive_strcmp(const unsigned char *string1, const unsigned char *string2)
{
    if ((string1 == NULL) || (string2 == NULL))
    {
        return 1;与系统头文件中的单行注释有关
    }

    if (string1 == string2)
    {
        return 0;
    }

    for(; tolower(*string1) == tolower(*string2); (void)string1++, string2++)
    {
        if (*string1 == '\0')
        {
            return 0;
        }
    }

    return tolower(*string1) - tolower(*string2);
}

typedef struct internal_hooks
{
    void *(CJSON_CDECL *allocate)(size_t size);
    void (CJSON_CDECL *deallocate)(void *pointer);
    void *(CJSON_CDECL *reallocate)(void *pointer, size_t size);
} internal_hooks;

#if defined(_MSC_VER)
/* work around MSVC error C2322: '...' address of dllimport '...' is not static */
static void * CJSON_CDECL internal_malloc(size_t size)
{
    return malloc(size);
}
static void CJSON_CDECL internal_free(void *pointer)
{
    free(pointer);
}
static void * CJSON_CDECL internal_realloc(void *pointer, size_t size)
{
    return realloc(pointer, size);
}
#else
#define internal_malloc malloc
#define internal_free free
#define internal_realloc realloc
#endif

/* strlen of character literals resolved at compile time */
#define static_strlen(string_literal) (sizeof(string_literal) - sizeof(""))

static internal_hooks global_hooks = { internal_malloc, internal_free, internal_realloc };

static unsigned char* cJSON_strdup(const unsigned char* string, const internal_hooks * const hooks)
{
    size_t length = 0;
    unsigned char *copy = NULL;

    if (string == NULL)
    {
        return NULL;
    }

    length = strlen((const char*)string) + sizeof("");
    copy = (unsigned char*)hooks->allocate(length);
    if (copy == NULL)
    {
        return NULL;
    }
    memcpy(copy, string, length);

    return copy;
}

CJSON_PUBLIC(void) cJSON_InitHooks(cJSON_Hooks* hooks)
{
    if (hooks == NULL)
    {
        /* Reset hooks */
        global_hooks.allocate = malloc;
        global_hooks.deallocate = free;
        global_hooks.reallocate = realloc;
        return;
    }

    global_hooks.allocate = malloc;
    if (hooks->malloc_fn != NULL)
    {
        global_hooks.allocate = hooks->malloc_fn;
    }

    global_hooks.deallocate = free;
    if (hooks->free_fn != NULL)
    {
        global_hooks.deallocate = hooks->free_fn;
    }

    /* use realloc only if both free and malloc are used */
    global_hooks.reallocate = NULL;
    if ((global_hooks.allocate == malloc) && (global_hooks.deallocate == free))
    {
        global_hooks.reallocate = realloc;
    }
}

/* Internal constructor. */
static cJSON *cJSON_New_Item(const internal_hooks * const hooks)
{
    cJSON* node = (cJSON*)hooks->allocate(sizeof(cJSON));
    if (node)
    {
        memset(node, '\0', sizeof(cJSON));
    }

    return node;
}

/* Delete a cJSON structure. */
CJSON_PUBLIC(void) cJSON_Delete(cJSON *item)
{
    cJSON *next = NULL;
    while (item != NULL)
    {
        next = item->next;
        if (!(item->type & cJSON_IsReference) && (item->child != NULL))
        {
            cJSON_Delete(item->child);
        }
        if (!(item->type & cJSON_IsReference) && (item->valuestring != NULL))
        {
            global_hooks.deallocate(item->valuestring);
            item->valuestring = NULL;
        }
        if (!(item->type & cJSON_StringIsConst) && (item->string != NULL))
        {
            global_hooks.deallocate(item->string);
            item->string = NULL;
        }
        global_hooks.deallocate(item);
        item = next;
    }
}

/* get the decimal point character of the current locale */
static unsigned char get_decimal_point(void)
{
#ifdef ENABLE_LOCALES
    struct lconv *lconv = localeconv();
    return (unsigned char) lconv->decimal_point[0];
#else
    return '.';
#endif
}

typedef struct
{
    const unsigned char *content;
    size_t length;
    size_t offset;
    size_t depth; /* How deeply nested (in arrays/objects) is the input at the current offset. */
    internal_hooks hooks;
} parse_buffer;

/* check if the given size is left to read in a given parse buffer (starting with 1) */
#define can_read(buffer, size) ((buffer != NULL) && (((buffer)->offset + size) <= (buffer)->length))
/* check if the buffer can be accessed at the given index (starting with 0) */
#define can_access_at_index(buffer, index) ((buffer != NULL) && (((buffer)->offset + index) < (buffer)->length))
#define cannot_access_at_index(buffer, index) (!can_access_at_index(buffer, index))
/* get a pointer to the buffer at the position */
#define buffer_at_offset(buffer) ((buffer)->content + (buffer)->offset)

/* Parse the input text to generate a number, and populate the result into item. */
static cJSON_bool parse_number(cJSON * const item, parse_buffer * const input_buffer)
{
    double number = 0;
    unsigned char *after_end = NULL;
    unsigned char *number_c_string;
    unsigned char decimal_point = get_decimal_point();
    size_t i = 0;
    size_t number_string_length = 0;
    cJSON_bool has_decimal_point = false;

    if ((input_buffer == NULL) || (input_buffer->content == NULL))
    {
        return false;
    }

    /* copy the number into a temporary buffer and replace '.' with the decimal point
     * of the current locale (for strtod)
     * This also takes care of '\0' not necessarily being available for marking the end of the input */
    for (i = 0; can_access_at_index(input_buffer, i); i++)
    {
        switch (buffer_at_offset(input_buffer)[i])
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '+':
            case '-':
            case 'e':
            case 'E':
                number_string_length++;
                break;

            case '.':
                number_string_length++;
                has_decimal_point = true;
                break;

            default:
                goto loop_end;
        }
    }
loop_end:
    /* malloc for temporary buffer, add 1 for '\0' */
    number_c_string = (unsigned char *) input_buffer->hooks.allocate(number_string_length + 1);
    if (number_c_string == NULL)
    {
        return false; /* allocation failure */
    }

    memcpy(number_c_string, buffer_at_offset(input_buffer), number_string_length);
    number_c_string[number_string_length] = '\0';

    if (has_decimal_point)
    {
        for (i = 0; i < number_string_length; i++)
        {
            if (number_c_string[i] == '.')
            {
                /* replace '.' with the decimal point of the current locale (for strtod) */
                number_c_string[i] = decimal_point;
            }
        }
    }

    number = strtod((const char*)number_c_string, (char**)&after_end);
    if (number_c_string == after_end)
    {
        /* free the temporary buffer */
        input_buffer->hooks.deallocate(number_c_string);
        return false; /* parse_error */
    }

    item->valuedouble = number;

    /* use saturation in case of overflow */
    if (number >= INT_MAX)
    {
        item->valueint = INT_MAX;
    }
    else if (number <= (double)INT_MIN)
    {
        item->valueint = INT_MIN;
    }
    else
    {
        item->valueint = (int)number;
    }

    item->type = cJSON_Number;

    input_buffer->offset += (size_t)(after_end - number_c_string);
    /* free the temporary buffer */
    input_buffer->hooks.deallocate(number_c_string);
    return true;
}

/* don't ask me, but the original cJSON_SetNumberValue returns an integer or double */
CJSON_PUBLIC(double) cJSON_SetNumberHelper(cJSON *object, double number)
{
    if (number >= INT_MAX)
    {
        object->valueint = INT_MAX;
    }
    else if (number <= (double)INT_MIN)
    {
        object->valueint = INT_MIN;
    }
    else
    {
        object->valueint = (int)number;
    }

    return object->valuedouble = number;
}

/* Note: when passing a NULL valuestring, cJSON_SetValuestring treats this as an error and return NULL */
CJSON_PUBLIC(char*) cJSON_SetValuestring(cJSON *object, const char *valuestring)
{
    char *copy = NULL;
    size_t v1_len;
    size_t v2_len;
    /* if object's type is not cJSON_String or is cJSON_IsReference, it should not set valuestring */
    if ((object == NULL) || !(object->type & cJSON_String) || (object->type & cJSON_IsReference))
    {
        return NULL;
    }
    /* return NULL if the object is corrupted or valuestring is NULL */
    if (object->valuestring == NULL || valuestring == NULL)
    {
        return NULL;
    }

    v1_len = strlen(valuestring);
    v2_len = strlen(object->valuestring);

    if (v1_len <= v2_len)
    {
        /* strcpy does not handle overlapping string: [X1, X2] [Y1, Y2] => X2 < Y1 or Y2 < X1 */
        if (!( valuestring + v1_len < object->valuestring || object->valuestring + v2_len < valuestring ))
        {
            return NULL;
        }
        strcpy(object->valuestring, valuestring);
        return object->valuestring;
    }
    copy = (char*) cJSON_strdup((const unsigned char*)valuestring, &global_hooks);
    if (copy == NULL)
    {
        return NULL;
    }
    if (object->valuestring != NULL)
    {
        cJSON_free(object->valuestring);
    }
    object->valuestring = copy;

    return copy;
}

typedef struct
{
    unsigned char *buffer;
    size_t length;
    size_t offset;
    size_t depth; /* current nesting depth (for formatted printing) */
    cJSON_bool noalloc;
    cJSON_bool format; /* is this print a formatted print */
    internal_hooks hooks;
} printbuffer;

/* realloc printbuffer if necessary to have at least "needed" bytes more */
static unsigned char* ensure(printbuffer * const p, size_t needed)
{
    unsigned char *newbuffer = NULL;
    size_t newsize = 0;

    if ((p == NULL) || (p->buffer == NULL))
    {
        return NULL;
    }

    if ((p->length > 0) && (p->offset >= p->length))
    {
        /* make sure that offset is valid */
        return NULL;
    }

    if (needed > INT_MAX)
    {
        /* sizes bigger than INT_MAX are currently not supported */
        return NULL;
    }

    needed += p->offset + 1;
    if (needed <= p->length)
    {
        return p->buffer + p->offset;
    }

    if (p->noalloc) {
        return NULL;
    }

    /* calculate new buffer size */
    if (needed > (INT_MAX / 2))
    {
        /* overflow of int, use INT_MAX if possible */
        if (needed <= INT_MAX)
        {
            newsize = INT_MAX;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        newsize = needed * 2;
    }

    if (p->hooks.reallocate != NULL)
    {
        /* reallocate with realloc if available */
        newbuffer = (unsigned char*)p->hooks.reallocate(p->buffer, newsize);
        if (newbuffer == NULL)
        {
            p->hooks.deallocate(p->buffer);
            p->length = 0;
            p->buffer = NULL;

            return NULL;
        }
    }
    else
    {
        /* otherwise reallocate manually */
        newbuffer = (unsigned char*)p->hooks.allocate(newsize);
        if (!newbuffer)
        {
            p->hooks.deallocate(p->buffer);
            p->length = 0;
            p->buffer = NULL;

            return NULL;
        }

        memcpy(newbuffer, p->buffer, p->offset + 1);
        p->hooks.deallocate(p->buffer);
    }
    p->length = newsize;
    p->buffer = newbuffer;

    return newbuffer + p->offset;
}

/* calculate the new length of the string in a printbuffer and update the offset */
static void update_offset(printbuffer * const buffer)
{
    const unsigned char *buffer_pointer = NULL;
    if ((buffer == NULL) || (buffer->buffer == NULL))
    {
        return;
    }
    buffer_pointer = buffer->buffer + buffer->offset;

    buffer->offset += strlen((const char*)buffer_pointer);
}

/* securely comparison of floating-point variables */
static cJSON_bool compare_double(double a, double b)
{
    double maxVal = fabs(a) > fabs(b) ? fabs(a) : fabs(b);
    return (fabs(a - b) <= maxVal * DBL_EPSILON);
}

/* Render the number nicely from the given item into a string. */
static cJSON_bool print_number(const cJSON * const item, printbuffer * const output_buffer)
{
    unsigned char *output_pointer = NULL;
    double d = item->valuedouble;
    int length = 0;
    size_t i = 0;
    unsigned char number_buffer[26] = {0}; /* temporary buffer to print the number into */
    unsigned char decimal_point = get_decimal_point();
    double test = 0.0;

    if (output_buffer == NULL)
    {
        return false;
    }

    /* This checks for NaN and Infinity */
    if (isnan(d) || isinf(d))
    {
        length = sprintf((char*)number_buffer, "null");
    }
    else if(d == (double)item->valueint)
    {
        length = sprintf((char*)number_buffer, "%d", item->valueint);
    }
    else
    {
        /* Try 15 decimal places of precision to avoid nonsignificant nonzero digits */
        length = sprintf((char*)number_buffer, "%1.15g", d);

        /* Check whether the original double can be recovered */
        if ((sscanf((char*)number_buffer, "%lg", &test) != 1) || !compare_double((double)test, d))
        {
            /* If not, print with 17 decimal places of precision */
            length = sprintf((char*)number_buffer, "%1.17g", d);
        }
    }

    /* sprintf failed or buffer overrun occurred */
    if ((length < 0) || (length > (int)(sizeof(number_buffer) - 1)))
    {
        return false;
    }

    /* reserve appropriate space in the output */
    output_pointer = ensure(output_buffer, (size_t)length + sizeof(""));
    if (output_pointer == NULL)
    {
        return false;
    }

    /* copy the printed number to the output and replace locale
     * dependent decimal point with '.' */
    for (i = 0; i < ((size_t)length); i++)
    {
        if (number_buffer[i] == decimal_point)
        {
            output_pointer[i] = '.';
            continue;
        }

        output_pointer[i] = number_buffer[i];
    }
    output_pointer[i] = '\0';

    output_buffer->offset += (size_t)length;

    return true;
}

/* parse 4 digit hexadecimal number */
static unsigned parse_hex4(const unsigned char * const input)
{
    unsigned int h = 0;
    size_t i = 0;

    for (i = 0; i < 4; i++)
    {
        /* parse digit */
        if ((input[i] >= '0') && (input[i] <= '9'))
        {
            h += (unsigned int) input[i] - '0';
        }
        else if ((input[i] >= 'A') && (input[i] <= 'F'))
        {
            h += (unsigned int) 10 + input[i] - 'A';
        }
        else if ((input[i] >= 'a') && (input[i] <= 'f'))
        {
            h += (unsigned int) 10 + input[i] - 'a';
        }
        else /* invalid */
        {
            return 0;
        }

        if (i < 3)
        {
            /* shift left to make place for the next nibble */
            h = h << 4;
        }
    }

    return h;
}

/* converts a UTF-16 literal to UTF-8
 * A literal can be one or two sequences of the form \uXXXX */
static unsigned char utf16_literal_to_utf8(const unsigned char * const input_pointer, const unsigned char * const input_end, unsigned char **output_pointer)
{
    long unsigned int codepoint = 0;
    unsigned int first_code = 0;
    const unsigned char *first_sequence = input_pointer;
    unsigned char utf8_length = 0;
    unsigned char utf8_position = 0;
    unsigned char sequence_length = 0;
    unsigned char first_byte_mark = 0;

    if ((input_end - first_sequence) < 6)
    {
        /* input ends unexpectedly */
        goto fail;
    }

    /* get the first utf16 sequence */
    first_code = parse_hex4(first_sequence + 2);

    /* check that the code is valid */
    if (((first_code >= 0xDC00) && (first_code <= 0xDFFF)))
    {
        goto fail;
    }

    /* UTF16 surrogate pair */
    if ((first_code >= 0xD800) && (first_code <= 0xDBFF))
    {
        const unsigned char *second_sequence = first_sequence + 6;
        unsigned int second_code = 0;
        sequence_length = 12; /* \uXXXX\uXXXX */

        if ((input_end - second_sequence) < 6)
        {
            /* input ends unexpectedly */
            goto fail;
        }

        if ((second_sequence[0] != '\\') || (second_sequence[1] != 'u'))
        {
            /* missing second half of the surrogate pair */
            goto fail;
        }

        /* get the second utf16 sequence */
        second_code = parse_hex4(second_sequence + 2);
        /* check that the code is valid */
        if ((second_code < 0xDC00) || (second_code > 0xDFFF))
        {
            /* invalid second half of the surrogate pair */
            goto fail;
        }


        /* calculate the unicode codepoint from the surrogate pair */
        codepoint = 0x10000 + (((first_code & 0x3FF) << 10) | (second_code & 0x3FF));
    }
    else
    {
        sequence_length = 6; /* \uXXXX */
        codepoint = first_code;
    }

    /* encode as UTF-8
     * takes at maximum 4 bytes to encode:
     * 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
    if (codepoint < 0x80)
    {
        /* normal ascii, encoding 0xxxxxxx */
        utf8_length = 1;
    }
    else if (codepoint < 0x800)
    {
        /* two bytes, encoding 110xxxxx 10xxxxxx */
        utf8_length = 2;
        first_byte_mark = 0xC0; /* 11000000 */
    }
    else if (codepoint < 0x10000)
    {
        /* three bytes, encoding 1110xxxx 10xxxxxx 10xxxxxx */
        utf8_length = 3;
        first_byte_mark = 0xE0; /* 11100000 */
    }
    else if (codepoint <= 0x10FFFF)
    {
        /* four bytes, encoding 1110xxxx 10xxxxxx 10xxxxxx 10xxxxxx */
        utf8_length = 4;
        first_byte_mark = 0xF0; /* 11110000 */
    }
    else
    {
        /* invalid unicode codepoint */
        goto fail;
    }

    /* encode as utf8 */
    for (utf8_position = (unsigned char)(utf8_length - 1); utf8_position > 0; utf8_position--)
    {
        /* 10xxxxxx */
        (*output_pointer)[utf8_position] = (unsigned char)((codepoint | 0x80) & 0xBF);
        codepoint >>= 6;
    }
    /* encode first byte */
    if (utf8_length > 1)
    {
        (*output_pointer)[0] = (unsigned char)((codepoint | first_byte_mark) & 0xFF);
    }
    else
    {
        (*output_pointer)[0] = (unsigned char)(codepoint & 0x7F);
    }

    *output_pointer += utf8_length;

    return sequence_length;

fail:
    return 0;
}

/* Parse the input text into an unescaped cinput, and populate item. */
static cJSON_bool parse_string(cJSON * const item, parse_buffer * const input_buffer)
{
    const unsigned char *input_pointer = buffer_at_offset(input_buffer) + 1;
    const unsigned char *input_end = buffer_at_offset(input_buffer) + 1;
    unsigned char *output_pointer = NULL;
    unsigned char *output = NULL;

    /* not a string */
    if (buffer_at_offset(input_buffer)[0] != '\"')
    {
        goto fail;
    }

    {
        /* calculate approximate size of the output (overestimate) */
        size_t allocation_length = 0;
        size_t skipped_bytes = 0;
        while (((size_t)(input_end - input_buffer->content) < input_buffer->length) && (*input_end != '\"'))
        {
            /* is escape sequence */
            if (input_end[0] == '\\')
            {
                if ((size_t)(input_end + 1 - input_buffer->content) >= input_buffer->length)
                {
                    /* prevent buffer overflow when last input character is a backslash */
                    goto fail;
                }
                skipped_bytes++;
                input_end++;
            }
            input_end++;
        }
        if (((size_t)(input_end - input_buffer->content) >= input_buffer->length) || (*input_end != '\"'))
        {
            goto fail; /* string ended unexpectedly */
        }

        /* This is at most how much we need for the output */
        allocation_length = (size_t) (input_end - buffer_at_offset(input_buffer)) - skipped_bytes;
        output = (unsigned char*)input_buffer->hooks.allocate(allocation_length + sizeof(""));
        if (output == NULL)
        {
            goto fail; /* allocation failure */
        }
    }

    output_pointer = output;
    /* loop through the string literal */
    while (input_pointer < input_end)
    {
        if (*input_pointer != '\\')
        {
            *output_pointer++ = *input_pointer++;
        }
        /* escape sequence */
        else
        {
            unsigned char sequence_length = 2;
            if ((input_end - input_pointer) < 1)
            {
                goto fail;
            }

            switch (input_pointer[1])
            {
                case 'b':
                    *output_pointer++ = '\b';
                    break;
                case 'f':
                    *output_pointer++ = '\f';
                    break;
                case 'n':
                    *output_pointer++ = '\n';
                    break;
                case 'r':
                    *output_pointer++ = '\r';
                    break;
                case 't':
                    *output_pointer++ = '\t';
                    break;
                case '\"':
                case '\\':与系统头文件中的单行注释有关
                case '/':
                    *output_pointer++ = input_pointer[1];
                    break;

                /* UTF-16 literal */
                case 'u':
                    sequence_length = utf16_literal_to_utf8(input_pointer, input_end, &output_pointer);
                    if (sequence_length == 0)
                    {
                        /* failed to convert UTF16-literal to UTF-8 */
                        goto fail;
                    }
                    break;

                default:
                    goto fail;
            }
            input_pointer += sequence_length;
        }
    }

    /* zero terminate the output */
    *output_pointer = '\0';

    item->type = cJSON_String;
    item->valuestring = (char*)output;

    input_buffer->offset = (size_t) (input_end - input_buffer->content);
    input_buffer->offset++;

    return true;

fail:
    if (output != NULL)
    {
        input_buffer->hooks.deallocate(output);
        output = NULL;
    }

    if (input_pointer != NULL)
    {
        input_buffer->offset = (size_t)(input_pointer - input_buffer->content);
    }

    return false;
}

/* Render the cstring provided to an escaped version that can be printed. */
static cJSON_bool print_string_ptr(const unsigned char * const input, printbuffer * const output_buffer)
{
    const unsigned char *input_pointer = NULL;
    unsigned char *output = NULL;
    unsigned char *output_pointer = NULL;
    size_t output_length = 0;
    /* numbers of additional characters needed for escaping */
    size_t escape_characters = 0;

    if (output_buffer == NULL)
    {
        return false;
    }

    /* empty string */
    if (input == NULL)
    {
        output = ensure(output_buffer, sizeof("\"\""));
        if (output == NULL)
        {
            return false;
        }
        strcpy((char*)output, "\"\"");

        return true;
    }

    /* set "flag" to 1 if something needs to be escaped */
    for (input_pointer = input; *input_pointer; input_与系统头文件中的单行注释有关pointer++)
    {
        switch (*input_pointer)
        {
            case '\"':
            case '\\':
            case '\b':
            case '\f':
            case '\n':
            case '\r':
            case '\t':
                /* one character escape sequence */
                escape_characters++;
                break;
            default:
                if (*input_pointer < 32)
                {
                    /* UTF-16 escape sequence uXXXX */
                    escape_characters += 5;
                }
                break;
        }
    }
    output_length = (size_t)(input_pointer - input) + escape_characters;

    output = ensure(output_buffer, output_length + sizeof("\"\""));
    if (output == NULL)
    {
        return false;
    }

    /* no characters have to be escaped */
    if (escape_characters == 0)
    {
        output[0] = '\"';
        memcpy(output + 1, input, output_length);
        output[output_length + 1] = '\"';
        output[output_length + 2] = '\0';

        return true;
    }

    output[0] = '\"';
    output_pointer = output + 1;
    /* copy the string */
    for (input_pointer = input; *input_pointer != '\0'; (void)input_pointer++, output_pointer++)
    {
        if ((*input_pointer > 31) && (*input_pointer != '\"') && (*input_pointer != '\\'))
        {
            /* normal character, copy */
            *output_pointer = *input_pointer;
        }
        else
        {
            /* character needs to be escaped */
            *output_pointer++ = '\\';
            switch (*input_pointer)
            {
                case '\\':
                    *output_pointer = '\\';
                    break;
                case '\"':
                    *output_pointer = '\"';
                    break;
                case '\b':
                    *output_pointer = 'b';
                    break;
                case '\f':
                    *output_pointer = 'f';
                    break;
                case '\n':
                    *output_pointer = 'n';
                    break;
                case '\r':
                    *output_pointer = 'r';
                    break;
                case '\t':
                    *output_pointer = 't';
                    break;
                default:
                    /* escape and print as unicode codepoint */
                    sprintf((char*)output_pointer, "u%04x", *input_pointer);
                    output_pointer += 4;
                    break;
            }
        }
    }
    output[output_length + 1] = '\"';
    output[output_length + 2] = '\0';

    return true;
}

/* Invoke print_string_ptr (which is useful) on an item. */
static cJSON_bool print_string(const cJSON * const item, printbuffer * const p)
{
    return print_string_ptr((unsigned char*)item->valuestring, p);
}

/* Predeclare these prototypes. */
static cJSON_bool parse_value(cJSON * const item, parse_buffer * const input_buffer);
static cJSON_bool print_value(const cJSON * const item, printbuffer * const output_buffer);
static cJSON_bool parse_array(cJSON * const item, parse_buffer * const input_buffer);
static cJSON_bool print_array(const cJSON * const item, printbuffer * const output_buffer);
static cJSON_bool parse_object(cJSON * const item, parse_buffer * const input_buffer);
static cJSON_bool print_object(const cJSON * const item, printbuffer * const output_buffer);

/* Utility to jump whitespace and cr/lf */
static parse_buffer *buffer_skip_whitespace(parse_buffer * const buffer)
{
    if ((buffer == NULL) || (buffer->content == NULL))
    {
        return NULL;
    }

    if (cannot_access_at_index(buffer, 0))
    {
        return buffer;
    }

    while (can_access_at_index(buffer, 0) && (buffer_at_offset(buffer)[0] <= 32))
    {
       buffer->offset++;
    }

    if (buffer->offset == buffer->length)
    {
        buffer->offset--;
    }

    return buffer;
}

/* skip the UTF-8 BOM (byte order mark) if it is at the beginning of a buffer */
static parse_buffer *skip_utf8_bom(parse_buffer * const buffer)
{
    if ((buffer == NULL) || (buffer->content == NULL) || (buffer->offset != 0))
    {
        return NULL;
    }

    if (can_access_at_index(buffer, 4) && (strncmp((const char*)buffer_at_offset(buffer), "\xEF\xBB\xBF", 3) == 0))
    {
        buffer->offset += 3;
    }

    return buffer;
}/*
 * 函数名：cJSON_ParseWithOpts
 * 作用：JSON 字符串解析的核心函数，将JSON格式字符串解析为cJSON树形结构
 * 参数：
 *    value：待解析的JSON字符串,必须以'\0'结尾，符合JSON语法规范
 *    return_parse_end：输出参数，解析完成后指向字符串中JSON数据结束的位置（可为NULL，表示不需要返回该位置）
 *    require_null_terminated：是否要求输入字符串必须以NULL结尾（1=要求，0=不要求）
 * 返回值：
 *    成功：返回解析后的cJSON树根节点指针
 *    失败：返回NULL（如内存分配失败、语法错误、空字符串等）
 * 内存管理规则：
 *    返回的cJSON树节点需要调用 cJSON_Delete() 手动释放，否则会造成内存泄漏
 *    解析过程中分配的临时节点，若解析失败会自动清理，不会泄漏
 */
CJSON_PUBLIC(cJSON *) cJSON_ParseWithOpts(const char *value,// 通用工具宏：取两个值中较小的那个
// 后续在拷贝缓冲区内容时会用到 避免拷贝超出缓冲区长度的内容 防止内存越界
#define cjson_min(a, b) (((a) < (b)) ? (a) : (b))

/*
 * 函数名：print
 * 作用：把cJSON节点转换成JSON格式的字符串 生成流程核心函数
 *       是上层cJSON_Print cJSON_PrintUnformatted等函数的底层实现
 *       承接上层传入的待转换节点和格式要求 调用下层print_value完成递归拼接 最后处理内存缓冲区
 * 参数：
 *   item：要转换的cJSON节点 上层函数如cJSON_Print会传入用户的根节点 传NULL则直接失败
 *   format：是否格式化输出 1带缩进换行 对应cJSON_Print 0紧凑格式 对应cJSON_PrintUnformatted
 *   hooks：内存操作函数集合 上层会传入全局默认的hooks 包含malloc realloc free 也支持用户自定义
 * 返回值：
 *   成功：返回生成的JSON字符串 需上层调用者用hooks->deallocate释放 如free
 *   失败：返回NULL 会自动清理内部分配的内存 上层无需处理
 * 内存管理规则：
 *   成功返回的字符串要调用hooks里的deallocate函数释放 不然会内存泄漏
 *   函数内部如果出错 会自动释放已经分配的内存 不会留垃圾
 * 逻辑关联：
 *   上层调用：cJSON_Print → print 本函数
 *   下层调用：本函数 → print_value 递归解析节点类型并拼接字符串 → update_offset 更新缓冲区写入位置
 */
static unsigned char *print(const cJSON * const item, cJSON_bool format, const internal_hooks * const hooks)
{
    // 默认的缓冲区初始大小 256字节 兼顾小JSON的效率和内存占用 大部分简单JSON无需扩容
    static const size_t default_buffer_size = 256;
    // 存储字符串拼接的缓冲区 后续会传给print_value 作为拼接JSON字符串的容器
    printbuffer buffer[1];
    // 最终返回给上层的JSON字符串 会根据缓冲区实际长度调整内存大小
    unsigned char *printed = NULL;

    // 把缓冲区结构体清空 避免内存里的脏数据影响后续的长度 偏移量等关键参数
    // 这是调用下层print_value前的必要准备 保证缓冲区初始状态干净
    memset(buffer, 0, sizeof(buffer));

    /* 第一步 初始化输出缓冲区 为下层print_value准备拼接容器 */
    // 用hooks分配初始缓冲区内存 后续print_value会往这个缓冲区里写JSON内容
    buffer->buffer = (unsigned char*) hooks->allocate(default_buffer_size);
    buffer->length = default_buffer_size;  // 记录缓冲区总长度 供print_value判断是否需要扩容
    buffer->format = format;               // 把格式化要求传给缓冲区 print_value会根据这个参数决定是否加缩进
    buffer->hooks = *hooks;                // 把内存操作函数绑定到缓冲区 print_value扩容时会用到
    // 内存分配失败直接跳转到fail 此时还没调用下层函数 只需释放当前缓冲区 本处还没有效内存
    if (buffer->buffer == NULL)
    {
        goto fail;
    }

    /* 第二步 核心逻辑 调用下层print_value完成JSON字符串拼接 */
    // print_value是递归函数 会根据item的类型 对象数组字符串等 拼接对应的JSON格式
    // 比如item是对象 就拼接key:value 是数组就拼接val1,val2 过程中会不断更新buffer->offset
    if (!print_value(item, buffer))
    {
        goto fail;  // 拼接失败 比如缓冲区扩容失败 跳转到fail清理内存
    }
    // 拼接完成后更新缓冲区的最终写入位置 确保offset准确反映实际的字符串长度
    // 这是为后续调整缓冲区大小做准备 避免分配的内存过长过短
    update_offset(buffer);

    /* 第三步 调整缓冲区大小 适配实际字符串长度 返回给上层 */
    // 场景1 如果支持内存重分配 hooks->reallocate不为NULL 对应标准的realloc
    // 直接把初始的256字节缓冲区调整为实际需要的大小 减少内存拷贝 效率更高
    if (hooks->reallocate != NULL)
    {
        // buffer->offset+1 +1是为了给字符串结束符'\0'预留位置 符合C字符串规范
        printed = (unsigned char*) hooks->reallocate(buffer->buffer, buffer->offset + 1);
        if (printed == NULL) {
            goto fail;  // 重分配失败 跳转到fail清理内存
        }
        // 把原缓冲区指针置空 避免后续fail标签里重复释放 同时移交内存所有权给printed
        buffer->buffer = NULL;
    }
    // 场景2 不支持重分配 用户自定义的hooks可能只实现了allocate deallocate
    // 新建一块刚好够存的内存 把内容拷贝过去 再释放原缓冲区
    else
    {
        // 分配刚好能存下拼接内容+结束符的内存 避免内存浪费
        printed = (unsigned char*) hooks->allocate(buffer->offset + 1);
        if (printed == NULL)
        {
            goto fail;  // 分配失败 跳转到fail清理内存
        }
        // 把缓冲区里的内容拷贝到新内存 用cjson_min避免拷贝超出缓冲区长度的内容 防止越界
        // 这里的buffer->length是初始的256 buffer->offset是实际拼接的长度 取较小值保证安全
        memcpy(printed, buffer->buffer, cjson_min(buffer->length, buffer->offset + 1));
        // 手动添加字符串结束符 即使print_value里没加 这里也兜底保证是合法的C字符串 方便上层使用
        printed[buffer->offset] = '\0';

        // 释放原来的256字节缓冲区 此时内容已拷贝到printed 原缓冲区没用了
        hooks->deallocate(buffer->buffer);
        buffer->buffer = NULL;  // 置空避免后续重复释放
    }

    // 转换成功 返回调整后的字符串给上层 比如cJSON_Print 上层会把unsigned char*转为char*返回给用户
    return printed;

// 失败处理逻辑 统一释放本函数内分配的所有内存 避免泄漏
// 无论哪一步失败 最终都会走到这里 保证内存清理干净
fail:
    // 释放初始的缓冲区内存 如果分配成功的话
    if (buffer->buffer != NULL)
    {
        hooks->deallocate(buffer->buffer);
        buffer->buffer = NULL;
    }

    // 释放调整后的字符串内存 如果分配成功的话
    if (printed != NULL)
    {
        hooks->deallocate(printed);
        printed = NULL;
    }

    // 转换失败返回NULL 上层函数如cJSON_Print会感知到失败 返回NULL给用户
    return NULL;
}

/* Render a cJSON item entity structure to text. */
// 该行是原代码的默认注释 说明此函数的核心用途 把cJSON结构渲染成文本 JSON字符串
// 对应上层对外暴露的cJSON_Print系列函数 是整个JSON生成流程的最终落地函数 const char **return_parse_end, cJSON_bool require_null_terminated)
{/* 定义缓冲区长度变量，用于存储带结束符的字符串总长度 */
    size_t buffer_length;
/* 关键行：空指针校验，避免传入NULL字符串导致后续strlen崩溃，是基础的健壮性保护 */
    if (NULL == value)
    {
        return NULL;
    }
/* 代码块：计算字符串总长度，包含末尾的NULL结束符
     * sizeof("") 等价于 sizeof(char) = 1，目的是显式为'\0'预留1字节空间
     * 即使require_null_terminated为0，也保证长度计算包含结束符，符合C字符串规范
     */
    /* Adding null character size due to require_null_terminated. */
    buffer_length = strlen(value) + sizeof("");
/* 关键行：调用底层核心解析函数，传递字符串、计算好的长度、输出参数和结束符要求
     * 所有真正的解析逻辑（递归解析、构建cJSON树）都在 cJSON_ParseWithLengthOpts 中完成
     */
    return cJSON_ParseWithLengthOpts(value, buffer_length, return_parse_end, require_null_terminated);
}

/*
 * 函数名：cJSON_ParseWithLengthOpts
 * 作用：cJSON 解析流程的真正核心实现函数 支持指定字符串长度解析JSON 是所有解析接口的底层依赖
 *       负责初始化解析缓冲区 创建根节点 调用递归解析逻辑 处理解析错误和返回结果
 * 参数：
 *   value：待解析的JSON字符串起始地址 允许非NULL结尾 但需配合buffer_length使用
 *   buffer_length：待解析字符串的总长度 字节数 包含可能的NULL结束符
 *   return_parse_end：输出参数 解析完成失败后指向字符串中解析停止的位置 可为NULL
 *   require_null_terminated：是否强制要求JSON字符串末尾必须是NULL '\0' 1要求 0不要求
 * 返回值：
 *   成功：返回解析后的cJSON树根节点指针 节点由malloc分配
 *   失败：返回NULL 如输入非法 内存分配失败 语法错误 不符合NULL结尾要求等
 * 内存管理规则：
 *   成功返回的cJSON节点需调用 cJSON_Delete 手动释放 否则造成内存泄漏
 *   解析失败时 本函数会自动释放已创建的cJSON节点 无需上层处理
 *   解析过程中依赖global_hooks指定的内存分配释放函数 默认是malloc free
*/

CJSON_PUBLIC(cJSON *) cJSON_ParseWithLengthOpts(const char *value, size_t buffer_length, const char **return_parse_end, cJSON_bool require_null_terminated)
{
    /* 初始化解析缓冲区结构体 content字符串地址 length总长度 offset当前解析位置 hooks内存钩子 error错误信息 */
    parse_buffer buffer 
= { 0, 0, 0, 0, { 0, 0, 0 } };
    /* 定义cJSON根节点指针 作为最终返回值 */
    cJSON 
*item = NULL;

    /* 关键行：重置全局错误信息 清除上一次解析的错误记录 保证本次解析的独立性 */
    /* reset error position */
    global_error
.json = NULL;
    global_error
.position = 0;

    /* 代码块：基础合法性校验 输入字符串为NULL或长度为0直接判定为解析失败 */
    if (value == NULL || 0 == buffer_length)
    {
        goto fail; /* 跳转到fail标签 统一处理失败逻辑 */
    }

    /* 代码块：初始化解析缓冲区核心参数 绑定待解析字符串 长度 当前偏移量和内存钩子 */
    buffer
.content = (const unsigned char*)value;  // 转换为unsigned char*方便字符操作
    buffer
.length = buffer_length;                // 设置字符串总长度
    buffer
.offset = 0;                            // 初始解析位置从字符串开头开始
    buffer
.hooks = global_hooks;                  // 绑定全局内存分配释放钩子 默认malloc free

    /* 代码块：创建cJSON根节点 作为解析结果的容器
     * cJSON_New_Item会调用hooks->malloc分配内存 失败则返回NULL
     */
    item 
= cJSON_New_Item(&global_hooks);
    if (item == NULL) /* memory fail */
    {
        goto fail; // 内存分配失败 跳转到失败处理逻辑
    }

    /* 代码块：调用递归解析函数 parse_value 解析JSON字符串并填充到根节点
     * 1 skip_utf8_bom(&buffer)：跳过UTF-8编码的BOM头 如0xEF 0xBB 0xBF 兼容特殊编码格式
     * 2 buffer_skip_whitespace(...)：跳过开头的空白字符 空格制表符换行回车 符合JSON规范
     * 3 parse_value(item, ...)：递归解析JSON值 对象数组字符串数字等 填充到根节点
     * 返回false表示解析失败 跳转到fail标签
     */
    if (!parse_value(item, buffer_skip_whitespace(skip_utf8_bom(&buffer))))
    {
        /* parse failure. ep is set. */
        goto fail;
    }

    /* 代码块：校验NULL结束符 仅当require_null_terminated=1时生效
     * 要求JSON字符串末尾必须是NULL 且不允许有多余的非空白字符 严格符合C字符串规范
     */
    /* if we require null-terminated JSON without appended garbage, skip and then check for a null terminator */
    if (require_null_terminated)
    {
        buffer_skip_whitespace(&buffer); // 先跳过解析后的空白字符
        // 检查：解析位置超出长度 或 当前位置不是NULL结束符 , 解析失败
        if ((buffer.offset >= buffer.length) || buffer_at_offset(&buffer)[0] != '\0')
        {
            goto fail;
        }
    }

    /* 代码块：设置输出参数return_parse_end 返回解析停止的位置 供上层调用者排查错误 */
    if (return_parse_end)
    {
        *return_parse_end = (const char*)buffer_at_offset(&buffer);
    }

    /* 解析成功 返回填充好的cJSON根节点 */
    return item;

/* 失败处理标签：统一释放资源 记录错误信息 返回NULL */
fail
:
    /* 若已创建cJSON节点 释放该节点 避免内存泄漏 */
    if (item != NULL)
    {
        cJSON_Delete(item);
    }

    /* 代码块：记录解析错误位置 供上层调用cJSON_GetErrorPtr获取错误信息 */
    if (value != NULL)
    {
        error local_error
;
        local_error
.json = (const unsigned char*)value; // 绑定错误对应的JSON字符串
        local_error
.position = 0;                      // 默认错误位置为字符串开头

        // 修正错误位置：优先取当前解析偏移量 若偏移量越界则取最后一个字符位置
        if (buffer.offset < buffer.length)
        {
            local_error
.position = buffer.offset;
        }
        else if (buffer.length > 0)
        {
            local_error
.position = buffer.length - 1;
        }

        // 若需要返回解析结束位置 设置为错误位置
        if (return_parse_end != NULL)
        {
            *return_parse_end = (const char*)local_error.json + local_error.position;
        }

        // 将本地错误信息更新到全局 供外部获取
        global_error 
= local_error;
    }

    /* 解析失败 返回NULL */
    return NULL;
}

/* Default options for cJSON_Parse */
/*
 * 函数名：cJSON_Parse
 * 作用：cJSON 最基础的JSON解析接口 为上层提供极简的调用方式
 *       直接封装调用cJSON_ParseWithOpts 采用默认参数解析JSON字符串
 * 参数：
 *   value：待解析的JSON字符串 C风格字符串 要求以NULL '\0'结尾
 * 返回值：
 *   成功：返回解析后的cJSON树根节点指针 节点由malloc分配
 *   失败：返回NULL 如输入NULL 内存分配失败 语法错误等
 * 内存管理规则：
 *   成功返回的cJSON节点需调用 cJSON_Delete 手动释放 否则造成内存泄漏
 *   本函数仅做参数封装 无直接内存分配 内存管理由底层函数负责
 */
CJSON_PUBLIC(cJSON *) cJSON_Parse(const char *value)
{
    /* 关键行：调用底层解析函数 传入默认参数
     * 第二个参数0：不返回解析结束位置
     * 第三个参数0：不强制要求字符串以NULL结尾
     */
    return cJSON_ParseWithOpts(value, 0, 0);
}

/*
 * 函数名：cJSON_ParseWithLength
 * 作用：支持指定长度的JSON解析接口 适配非NULL结尾的内存块解析场景
 *       封装调用cJSON_ParseWithLengthOpts 采用默认参数简化调用
 * 参数：
 *   value：待解析的JSON字符串起始地址
 *   buffer_length：待解析字符串的总长度 字节数
 * 返回值：
 *   成功：返回解析后的cJSON树根节点指针 节点由malloc分配
 *   失败：返回NULL 如输入NULL 长度为0 内存分配失败 语法错误等
 * 内存管理规则：
 *   成功返回的cJSON节点需调用 cJSON_Delete 手动释放 否则造成内存泄漏
 *   本函数仅做参数封装 无直接内存分配 内存管理由底层函数负责
 */
CJSON_PUBLIC(cJSON *) cJSON_ParseWithLength(const char *value, size_t buffer_length)
{
    /* 关键行：调用底层核心解析函数 传入默认参数
     * 第三个参数0：不返回解析结束位置
     * 第四个参数0：不强制要求字符串以NULL结尾
     */
    return cJSON_ParseWithLengthOpts(value, buffer_length, 0, 0);
}
与系统头文件中的单行注释有关
// 通用工具宏：取两个值中较小的那个
// 后续在拷贝缓冲区内容时会用到 避免拷贝超出缓冲区长度的内容 防止内存越界
#define cjson_min(a, b) (((a) < (b)) ? (a) : (b))

/*
 * 函数名：print
 * 作用：把cJSON节点转换成JSON格式的字符串 生成流程核心函数
 *       是上层cJSON_Print cJSON_PrintUnformatted等函数的底层实现
 *       承接上层传入的待转换节点和格式要求 调用下层print_value完成递归拼接 最后处理内存缓冲区
 * 参数：
 *   item：要转换的cJSON节点 上层函数如cJSON_Print会传入用户的根节点 传NULL则直接失败
 *   format：是否格式化输出 1带缩进换行 对应cJSON_Print 0紧凑格式 对应cJSON_PrintUnformatted
 *   hooks：内存操作函数集合 上层会传入全局默认的hooks 包含malloc realloc free 也支持用户自定义
 * 返回值：
 *   成功：返回生成的JSON字符串 需上层调用者用hooks->deallocate释放 如free
 *   失败：返回NULL 会自动清理内部分配的内存 上层无需处理
 * 内存管理规则：
 *   成功返回的字符串要调用hooks里的deallocate函数释放 不然会内存泄漏
 *   函数内部如果出错 会自动释放已经分配的内存 不会留垃圾
 * 逻辑关联：
 *   上层调用：cJSON_Print → print 本函数
 *   下层调用：本函数 → print_value 递归解析节点类型并拼接字符串 → update_offset 更新缓冲区写入位置
 */
static unsigned char *print(const cJSON * const item, cJSON_bool format, const internal_hooks * const hooks)
{
    // 默认的缓冲区初始大小 256字节 兼顾小JSON的效率和内存占用 大部分简单JSON无需扩容
    static const size_t default_buffer_size = 256;
    // 存储字符串拼接的缓冲区 后续会传给print_value 作为拼接JSON字符串的容器
    printbuffer buffer[1];
    // 最终返回给上层的JSON字符串 会根据缓冲区实际长度调整内存大小
    unsigned char *printed = NULL;

    // 把缓冲区结构体清空 避免内存里的脏数据影响后续的长度 偏移量等关键参数
    // 这是调用下层print_value前的必要准备 保证缓冲区初始状态干净
    memset(buffer, 0, sizeof(buffer));

    /* 第一步 初始化输出缓冲区 为下层print_value准备拼接容器 */
    // 用hooks分配初始缓冲区内存 后续print_value会往这个缓冲区里写JSON内容
    buffer->buffer = (unsigned char*) hooks->allocate(default_buffer_size);
    buffer->length = default_buffer_size;  // 记录缓冲区总长度 供print_value判断是否需要扩容
    buffer->format = format;               // 把格式化要求传给缓冲区 print_value会根据这个参数决定是否加缩进
    buffer->hooks = *hooks;                // 把内存操作函数绑定到缓冲区 print_value扩容时会用到
    // 内存分配失败直接跳转到fail 此时还没调用下层函数 只需释放当前缓冲区 本处还没有效内存
    if (buffer->buffer == NULL)
    {
        goto fail;
    }

    /* 第二步 核心逻辑 调用下层print_value完成JSON字符串拼接 */
    // print_value是递归函数 会根据item的类型 对象数组字符串等 拼接对应的JSON格式
    // 比如item是对象 就拼接key:value 是数组就拼接val1,val2 过程中会不断更新buffer->offset
    if (!print_value(item, buffer))
    {
        goto fail;  // 拼接失败 比如缓冲区扩容失败 跳转到fail清理内存
    }
    // 拼接完成后更新缓冲区的最终写入位置 确保offset准确反映实际的字符串长度
    // 这是为后续调整缓冲区大小做准备 避免分配的内存过长过短
    update_offset(buffer);

    /* 第三步 调整缓冲区大小 适配实际字符串长度 返回给上层 */
    // 场景1 如果支持内存重分配 hooks->reallocate不为NULL 对应标准的realloc
    // 直接把初始的256字节缓冲区调整为实际需要的大小 减少内存拷贝 效率更高
    if (hooks->reallocate != NULL)
    {
        // buffer->offset+1 +1是为了给字符串结束符'\0'预留位置 符合C字符串规范
        printed = (unsigned char*) hooks->reallocate(buffer->buffer, buffer->offset + 1);
        if (printed == NULL) {
            goto fail;  // 重分配失败 跳转到fail清理内存
        }
        // 把原缓冲区指针置空 避免后续fail标签里重复释放 同时移交内存所有权给printed
        buffer->buffer = NULL;
    }
    // 场景2 不支持重分配 用户自定义的hooks可能只实现了allocate deallocate
    // 新建一块刚好够存的内存 把内容拷贝过去 再释放原缓冲区
    else
    {
        // 分配刚好能存下拼接内容+结束符的内存 避免内存浪费
        printed = (unsigned char*) hooks->allocate(buffer->offset + 1);
        if (printed == NULL)
        {
            goto fail;  // 分配失败 跳转到fail清理内存
        }
        // 把缓冲区里的内容拷贝到新内存 用cjson_min避免拷贝超出缓冲区长度的内容 防止越界
        // 这里的buffer->length是初始的256 buffer->offset是实际拼接的长度 取较小值保证安全
        memcpy(printed, buffer->buffer, cjson_min(buffer->length, buffer->offset + 1));
        // 手动添加字符串结束符 即使print_value里没加 这里也兜底保证是合法的C字符串 方便上层使用
        printed[buffer->offset] = '\0';

        // 释放原来的256字节缓冲区 此时内容已拷贝到printed 原缓冲区没用了
        hooks->deallocate(buffer->buffer);
        buffer->buffer = NULL;  // 置空避免后续重复释放
    }

    // 转换成功 返回调整后的字符串给上层 比如cJSON_Print 上层会把unsigned char*转为char*返回给用户
    return printed;

// 失败处理逻辑 统一释放本函数内分配的所有内存 避免泄漏
// 无论哪一步失败 最终都会走到这里 保证内存清理干净
fail:
    // 释放初始的缓冲区内存 如果分配成功的话
    if (buffer->buffer != NULL)
    {
        hooks->deallocate(buffer->buffer);
        buffer->buffer = NULL;
    }

    // 释放调整后的字符串内存 如果分配成功的话
    if (printed != NULL)
    {
        hooks->deallocate(printed);
        printed = NULL;
    }

    // 转换失败返回NULL 上层函数如cJSON_Print会感知到失败 返回NULL给用户
    return NULL;
}

/* Render a cJSON item entity structure to text. */
// 该行是原代码的默认注释 说明此函数的核心用途 把cJSON结构渲染成文本 JSON字符串
// 对应上层对外暴露的cJSON_Print系列函数 是整个JSON生成流程的最终落地函数
/*
 * 函数名：cJSON_Print
 * 作用：对外暴露的格式化输出JSON字符串接口
 *       是print底层函数的上层封装 专门返回带缩进换行的易读格式JSON
 * 参数：
 *   item：待转换的cJSON根节点 由用户传入 传NULL则返回NULL
 * 返回值：
 *   成功：返回格式化的JSON字符串 需调用free手动释放
 *   失败：返回NULL
 * 逻辑关联：
 *   调用下层：cJSON_Print → print 传入format=true 启用格式化输出
 *   后续处理：用户拿到字符串使用后 必须用free释放 否则内存泄漏
 */
CJSON_PUBLIC(char *) cJSON_Print(const cJSON *item)
{
    // 调用底层print函数 第二个参数true表示格式化输出 传入全局默认内存操作函数
    // 强制转换为char*是因为底层返回unsigned char* 适配上层对外的接口规范
    return (char*)print(item, true, &global_hooks);
}

/*
 * 函数名：cJSON_PrintUnformatted
 * 作用：对外暴露的紧凑格式输出JSON字符串接口
 *       是print底层函数的上层封装 返回无缩进无换行的紧凑JSON 节省存储空间
 * 参数：
 *   item：待转换的cJSON根节点 由用户传入 传NULL则返回NULL
 * 返回值：
 *   成功：返回紧凑格式的JSON字符串 需调用free手动释放
 *   失败：返回NULL
 * 逻辑关联：
 *   调用下层：cJSON_PrintUnformatted → print 传入format=false 禁用格式化输出
 *   适用场景：网络传输 存储等需要减小JSON体积的场景
 */
CJSON_PUBLIC(char *) cJSON_PrintUnformatted(const cJSON *item)
{
    // 调用底层print函数 第二个参数false表示紧凑输出 无缩进换行
    return (char*)print(item, false, &global_hooks);
}

/*
 * 函数名：cJSON_PrintBuffered
 * 作用：对外暴露的预分配缓冲区输出JSON接口
 *       允许用户指定初始缓冲区大小 减少底层自动扩容的次数 提升效率
 * 参数：
 *   item：待转换的cJSON根节点 传NULL则返回NULL
 *   prebuffer：初始缓冲区大小 字节数 传负数直接返回NULL
 *   fmt：是否格式化输出 1格式化 0紧凑格式
 * 返回值：
 *   成功：返回生成的JSON字符串 需调用free手动释放
 *   失败：返回NULL 会自动释放已分配的缓冲区
 * 逻辑关联：
 *   下层调用：直接调用print_value完成字符串拼接 不经过print函数的内存调整逻辑
 *   区别于print：本函数自己管理初始缓冲区 避免print函数中可能的内存重分配
 */
CJSON_PUBLIC(char *) cJSON_PrintBuffered(const cJSON *item, int prebuffer, cJSON_bool fmt)
{
    // 定义输出缓冲区 用于拼接JSON字符串
    printbuffer p = { 0, 0, 0, 0, 0, 0, { 0, 0, 0 } };

    // 预分配大小不能为负数 非法参数直接返回NULL
    if (prebuffer < 0)
    {
        return NULL;
    }

    // 用全局内存函数分配指定大小的初始缓冲区
    p.buffer = (unsigned char*)global_hooks.allocate((size_t)prebuffer);
    // 内存分配失败返回NULL
    if (!p.buffer)
    {
        return NULL;
    }

    // 初始化缓冲区参数
    p.length = (size_t)prebuffer;  // 缓冲区总长度
    p.offset = 0;                  // 初始写入位置
    p.noalloc = false;             // 允许缓冲区自动扩容
    p.format = fmt;                // 格式化开关
    p.hooks = global_hooks;        // 绑定内存操作函数

    // 调用print_value拼接JSON字符串 失败则释放缓冲区并返回NULL
    if (!print_value(item, &p))
    {
        global_hooks.deallocate(p.buffer);
        p.buffer = NULL;
        return NULL;
    }

    // 成功则返回缓冲区指针 转换为char*适配上层接口
    return (char*)p.buffer;
}

/*
 * 函数名：cJSON_PrintPreallocated
 * 作用：对外暴露的使用用户预分配缓冲区输出JSON接口
 *       不内部分配内存 直接使用用户提供的缓冲区 避免内存拷贝 效率最高
 * 参数：
 *   item：待转换的cJSON根节点
 *   buffer：用户提供的缓冲区指针 传NULL返回false
 *   length：缓冲区长度 字节数 传负数返回false
 *   format：是否格式化输出 1格式化 0紧凑格式
 * 返回值：
 *   成功：返回true 缓冲区中已写入JSON字符串
 *   失败：返回false 缓冲区内容不可用
 * 逻辑关联：
 *   下层调用：直接调用print_value 把内容写入用户缓冲区
 *   注意事项：用户需保证缓冲区大小足够 否则会写入失败
 *   区别于其他接口：本函数无内存分配 无需释放 由用户自行管理缓冲区
 */
CJSON_PUBLIC(cJSON_bool) cJSON_PrintPreallocated(cJSON *item, char *buffer, const int length, const cJSON_bool format)
{
    // 定义输出缓冲区 绑定到用户提供的内存
    printbuffer p = { 0, 0, 0, 0, 0, 0, { 0, 0, 0 } };

    // 参数合法性校验 缓冲区为NULL或长度为负直接返回失败
    if ((length < 0) || (buffer == NULL))
    {
        return false;
    }

    // 绑定用户提供的缓冲区
    p.buffer = (unsigned char*)buffer;
    p.length = (size_t)length;     // 缓冲区长度由用户指定
    p.offset = 0;                  // 初始写入位置
    p.noalloc = true;              // 禁止自动扩容 避免修改用户内存
    p.format = format;             // 格式化开关
    p.hooks = global_hooks;        // 绑定内存操作函数（仅用于内部判断 无实际分配）

    // 调用print_value写入内容 直接返回其结果
    return print_value(item, &p);
}

/* Parser core - when encountering text, process appropriately. */
/*
 * 函数名：parse_value
 * 作用：解析流程核心函数 识别并解析JSON中的各种值类型
 *       是parse_array parse_object的下层依赖 递归解析每个JSON值
 * 参数：
 *   item：存储解析结果的cJSON节点 由上层分配
 *   input_buffer：解析缓冲区 包含待解析字符串和当前位置
 * 返回值：
 *   成功：返回true item中已填充对应的值类型和数据
 *   失败：返回false 解析出错或无输入
 * 逻辑关联：
 *   上层调用：parse_array parse_object cJSON_ParseWithLengthOpts → parse_value
 *   下层调用：parse_value → parse_string parse_number parse_array parse_object
 *   解析顺序：按JSON规范依次匹配null false true 字符串 数字 数组 对象
 */
static cJSON_bool parse_value(cJSON * const item, parse_buffer * const input_buffer)
{
    // 缓冲区为空直接返回失败 无输入内容可解析
    if ((input_buffer == NULL) || (input_buffer->content == NULL))
    {
        return false; /* no input */
    }

    /* 按JSON规范依次解析不同类型的值 */
    /* 解析null值 */
    // 检查缓冲区剩余长度足够 且当前位置是"null"字符串
    if (can_read(input_buffer, 4) && (strncmp((const char*)buffer_at_offset(input_buffer), "null", 4) == 0))
    {
        item->type = cJSON_NULL;    // 设置节点类型为null
        input_buffer->offset += 4;  // 移动解析指针 跳过"null"
        return true;
    }
    /* 解析false值 */
    if (can_read(input_buffer, 5) && (strncmp((const char*)buffer_at_offset(input_buffer), "false", 5) == 0))
    {
        item->type = cJSON_False;   // 设置节点类型为false
        input_buffer->offset += 5;  // 移动解析指针 跳过"false"
        return true;
    }
    /* 解析true值 */
    if (can_read(input_buffer, 4) && (strncmp((const char*)buffer_at_offset(input_buffer), "true", 4) == 0))
    {
        item->type = cJSON_True;    // 设置节点类型为true
        item->valueint = 1;         // 给整型值赋值1 方便用户获取
        input_buffer->offset += 4;  // 移动解析指针 跳过"true"
        return true;
    }
    /* 解析字符串值 */
    // 当前字符是双引号 符合JSON字符串的起始标识
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '\"'))
    {
        // 调用parse_string解析字符串内容 结果存入item
        return parse_string(item, input_buffer);
    }
    /* 解析数字值 */
    // 数字以减号或0-9开头 符合JSON数字的起始规则
    if (can_access_at_index(input_buffer, 0) && ((buffer_at_offset(input_buffer)[0] == '-') || ((buffer_at_offset(input_buffer)[0] >= '0') && (buffer_at_offset(input_buffer)[0] <= '9'))))
    {
        // 调用parse_number解析数字内容 结果存入item
        return parse_number(item, input_buffer);
    }
    /* 解析数组值 */
    // 当前字符是左中括号 符合JSON数组的起始标识
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '['))
    {
        // 调用parse_array解析数组内容 结果存入item
        return parse_array(item, input_buffer);
    }
    /* 解析对象值 */
    // 当前字符是左大括号 符合JSON对象的起始标识
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '{'))
    {
        // 调用parse_object解析对象内容 结果存入item
        return parse_object(item, input_buffer);
    }

    // 匹配不到任何JSON值类型 解析失败
    return false;
}

/* Render a value to text. */
/*
 * 函数名：print_value
 * 作用：生成流程核心函数 根据cJSON节点类型拼接对应的JSON字符串
 *       是print print_array print_object的下层依赖 递归处理每个节点
 * 参数：
 *   item：待转换的cJSON节点 传NULL返回false
 *   output_buffer：输出缓冲区 存储拼接的字符串
 * 返回值：
 *   成功：返回true 缓冲区中已写入对应类型的JSON字符串
 *   失败：返回false 写入失败
 * 逻辑关联：
 *   上层调用：print print_array print_object → print_value
 *   下层调用：print_value → print_number print_string print_array print_object
 *   处理逻辑：按节点类型分支处理 每种类型拼接对应的JSON格式字符串
 */
static cJSON_bool print_value(const cJSON * const item, printbuffer * const output_buffer)
{
    // 指向缓冲区当前写入位置的指针
    unsigned char *output = NULL;

    // 节点或缓冲区为空 直接返回失败
    if ((item == NULL) || (output_buffer == NULL))
    {
        return false;
    }

    // 按节点类型分支处理 仅取低8位避免类型标识冲突
    switch ((item->type) & 0xFF)
    {
        case cJSON_NULL:
            // 确保缓冲区有足够空间 存储"null"+结束符共5字节
            output = ensure(output_buffer, 5);
            if (output == NULL)
            {
                return false;
            }
            // 写入"null"字符串
            strcpy((char*)output, "null");
            return true;

        case cJSON_False:
            // 确保缓冲区有足够空间 存储"false"+结束符共6字节
            output = ensure(output_buffer, 6);
            if (output == NULL)
            {
                return false;
            }
            // 写入"false"字符串
            strcpy((char*)output, "false");
            return true;

        case cJSON_True:
            // 确保缓冲区有足够空间 存储"true"+结束符共5字节
            output = ensure(output_buffer, 5);
            if (output == NULL)
            {
                return false;
            }
            // 写入"true"字符串
            strcpy((char*)output, "true");
            return true;

        case cJSON_Number:
            // 调用print_number处理数字类型 拼接数字字符串
            return print_number(item, output_buffer);

        case cJSON_Raw:
        {
            // 存储原始字符串长度
            size_t raw_length = 0;
            // 原始字符串为空 返回失败
            if (item->valuestring == NULL)
            {
                return false;
            }

            // 计算原始字符串长度 包含结束符
            raw_length = strlen(item->valuestring) + sizeof("");
            // 确保缓冲区有足够空间
            output = ensure(output_buffer, raw_length);
            if (output == NULL)
            {
                return false;
            }
            // 拷贝原始字符串到缓冲区
            memcpy(output, item->valuestring, raw_length);
            return true;
        }

        case cJSON_String:
            // 调用print_string处理字符串类型 拼接带双引号的字符串
            return print_string(item, output_buffer);

        case cJSON_Array:
            // 调用print_array处理数组类型 拼接数组格式字符串
            return print_array(item, output_buffer);

        case cJSON_Object:
            // 调用print_object处理对象类型 拼接对象格式字符串
            return print_object(item, output_buffer);

        // 未知节点类型 返回失败
        default:
            return false;
    }
}

/* Build an array from input text. */
/*
 * 函数名：parse_array
 * 作用：解析JSON数组 构建对应的cJSON数组节点
 *       是parse_value的下层函数 递归解析数组中的每个元素
 * 参数：
 *   item：存储数组结果的cJSON节点 由上层分配
 *   input_buffer：解析缓冲区 包含待解析字符串和当前位置
 * 返回值：
 *   成功：返回true item中已填充数组节点和子元素
 *   失败：返回false 解析出错 会自动释放已分配的子节点
 * 逻辑关联：
 *   上层调用：parse_value → parse_array
 *   下层调用：parse_array → parse_value 递归解析每个数组元素
 *   解析规则：匹配[]包裹的逗号分隔元素 支持空数组 处理嵌套数组
 */
static cJSON_bool parse_array(cJSON * const item, parse_buffer * const input_buffer)
{
    // 数组链表的头节点
    cJSON *head = NULL;
    // 当前处理的数组元素节点
    cJSON *current_item = NULL;

    // 检查嵌套深度 超过限制返回失败 防止栈溢出
    if (input_buffer->depth >= CJSON_NESTING_LIMIT)
    {
        return false; /* to deeply nested */
    }
    // 嵌套深度加1 处理当前数组
    input_buffer->depth++;

    // 当前字符不是左中括号 不是数组 解析失败
    if (buffer_at_offset(input_buffer)[0] != '[')
    {
        /* not an array */
        goto fail;
    }

    // 移动解析指针 跳过左中括号
    input_buffer->offset++;
    // 跳过空白字符 符合JSON规范
    buffer_skip_whitespace(input_buffer);
    // 当前字符是右中括号 空数组 直接跳转到成功处理
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ']'))
    {
        /* empty array */
        goto success;
    }

    /* 检查是否解析到缓冲区末尾 */
    if (cannot_access_at_index(input_buffer, 0))
    {
        input_buffer->offset--;
        goto fail;
    }

    /* 回退指针到第一个元素前的位置 为循环解析做准备 */
    input_buffer->offset--;
    /* 循环解析逗号分隔的数组元素 */
    do
    {
        /* 分配新的元素节点 */
        cJSON *new_item = cJSON_New_Item(&(input_buffer->hooks));
        if (new_item == NULL)
        {
            goto fail; /* allocation failure */
        }

        /* 将新节点加入链表 */
        if (head == NULL)
        {
            /* 初始化链表头节点 */
            current_item = head = new_item;
        }
        else
        {
            /* 追加到链表尾部 并移动当前节点指针 */
            current_item->next = new_item;
            new_item->prev = current_item;
            current_item = new_item;
        }

        /* 解析下一个元素值 */
        input_buffer->offset++;
        buffer_skip_whitespace(input_buffer);
        // 调用parse_value解析元素内容 失败则清理
        if (!parse_value(current_item, input_buffer))
        {
            goto fail; /* failed to parse value */
        }
        buffer_skip_whitespace(input_buffer);
    }
    // 只要当前字符是逗号 就继续解析下一个元素
    while (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ','));

    // 解析到缓冲区末尾 或 当前字符不是右中括号 数组格式错误
    if (cannot_access_at_index(input_buffer, 0) || buffer_at_offset(input_buffer)[0] != ']')
    {
        goto fail; /* expected end of array */
    }

success:
    // 嵌套深度减1 完成当前数组解析
    input_buffer->depth--;

    // 链表不为空时 把尾节点的prev指向头节点 形成循环链表
    if (head != NULL) {
        head->prev = current_item;
    }

    // 设置当前节点为数组类型
    item->type = cJSON_Array;
    // 绑定数组的子节点链表
    item->child = head;

    // 移动解析指针 跳过右中括号
    input_buffer->offset++;

    return true;

fail:
    // 解析失败 释放已分配的数组元素链表
    if (head != NULL)
    {
        cJSON_Delete(head);
    }

    return false;
}

/* Render an array to text */
/*
 * 函数名：print_array
 * 作用：将cJSON数组节点转换为JSON数组格式字符串
 *       是print_value的下层函数 递归处理数组中的每个元素
 * 参数：
 *   item：待转换的cJSON数组节点
 *   output_buffer：输出缓冲区 存储拼接的字符串
 * 返回值：
 *   成功：返回true 缓冲区中已写入数组格式字符串
 *   失败：返回false 写入失败
 * 逻辑关联：
 *   上层调用：print_value → print_array
 *   下层调用：print_array → print_value 递归处理每个数组元素
 *   生成规则：拼接[]包裹的逗号分隔元素 格式化模式下加空格和缩进
 */
static cJSON_bool print_array(const cJSON * const item, printbuffer * const output_buffer)
{
    // 指向缓冲区当前写入位置的指针
    unsigned char *output_pointer = NULL;
    // 存储需要的缓冲区长度
    size_t length = 0;
    // 遍历数组的当前元素节点
    cJSON *current_element = item->child;

    // 缓冲区为空 返回失败
    if (output_buffer == NULL)
    {
        return false;
    }

    /* 拼接输出数组字符串 */
    /* 写入左中括号 */
    // 确保缓冲区有至少1字节空间
    output_pointer = ensure(output_buffer, 1);
    if (output_pointer == NULL)
    {
        return false;
    }

    *output_pointer = '[';         // 写入左中括号
    output_buffer->offset++;       // 更新缓冲区写入位置
    output_buffer->depth++;        // 嵌套深度加1 用于格式化缩进

    // 遍历数组的所有元素
    while (current_element != NULL)
    {
        // 调用print_value转换当前元素为字符串
        if (!print_value(current_element, output_buffer))
        {
            return false;
        }
        // 更新缓冲区写入位置
        update_offset(output_buffer);
        // 当前元素不是最后一个 写入分隔符
        if (current_element->next)
        {
            // 计算需要的空间 格式化模式需要2字节 逗号+空格 紧凑模式1字节 仅逗号
            length = (size_t) (output_buffer->format ? 2 : 1);
            // 确保缓冲区有足够空间
            output_pointer = ensure(output_buffer, length + 1);
            if (output_pointer == NULL)
            {
                return false;
            }
            *output_pointer++ = ','; // 写入逗号
            if(output_buffer->format)
            {
                *output_pointer++ = ' '; // 格式化模式加空格
            }
            *output_pointer = '\0';      // 写入字符串结束符
            output_buffer->offset += length; // 更新写入位置
        }
        // 移动到下一个元素
        current_element = current_element->next;
    }

    // 写入右中括号 需2字节空间 右中括号+结束符
    output_pointer = ensure(output_buffer, 2);
    if (output_pointer == NULL)
    {
        return false;
    }
    *output_pointer++ = ']'; // 写入右中括号
    *output_pointer = '\0';  // 写入结束符
    output_buffer->depth--;  // 嵌套深度减1

    return true;
}

/* Build an object from the text. */
/*
 * 函数名：parse_object
 * 作用：解析JSON对象 构建对应的cJSON对象节点
 *       是parse_value的下层函数 递归解析对象中的键值对
 * 参数：
 *   item：存储对象结果的cJSON节点 由上层分配
 *   input_buffer：解析缓冲区 包含待解析字符串和当前位置
 * 返回值：
 *   成功：返回true item中已填充对象节点和键值对子元素
 *   失败：返回false 解析出错 会自动释放已分配的子节点
 * 逻辑关联：
 *   上层调用：parse_value → parse_object
 *   下层调用：parse_object → parse_string 解析键名 → parse_value 解析值
 *   解析规则：匹配{}包裹的逗号分隔键值对 支持空对象 处理嵌套对象 键名必须是字符串
 */
static cJSON_bool parse_object(cJSON * const item, parse_buffer * const input_buffer)
{
    // 对象键值对链表的头节点
    cJSON *head = NULL;
    // 当前处理的键值对节点
    cJSON *current_item = NULL;

    // 检查嵌套深度 超过限制返回失败 防止栈溢出
    if (input_buffer->depth >= CJSON_NESTING_LIMIT)
    {
        return false; /* to deeply nested */
    }
    // 嵌套深度加1 处理当前对象
    input_buffer->depth++;

    // 缓冲区无内容 或 当前字符不是左大括号 不是对象 解析失败
    if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != '{'))
    {
        goto fail; /* not an object */
    }

    // 移动解析指针 跳过左大括号
    input_buffer->offset++;
    // 跳过空白字符 符合JSON规范
    buffer_skip_whitespace(input_buffer);
    // 当前字符是右大括号 空对象 直接跳转到成功处理
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '}'))
    {
        goto success; /* empty object */
    }

    /* 检查是否解析到缓冲区末尾 */
    if (cannot_access_at_index(input_buffer, 0))
    {
        input_buffer->offset--;
        goto fail;
    }

    /* 回退指针到第一个键值对前的位置 为循环解析做准备 */
    input_buffer->offset--;
    /* 循环解析逗号分隔的对象键值对 */
    do
    {
        /* 分配新的键值对节点 */
        cJSON *new_item = cJSON_New_Item(&(input_buffer->hooks));
        if (new_item == NULL)
        {
            goto fail; /* allocation failure */
        }

        /* 将新节点加入链表 */
        if (head == NULL)
        {
            /* 初始化链表头节点 */
            current_item = head = new_item;
        }
        else
        {
            /* 追加到链表尾部 并移动当前节点指针 */
            current_item->next = new_item;
            new_item->prev = current_item;
            current_item = new_item;
        }

        // 检查逗号后是否有内容 无内容则对象格式错误
        if (cannot_access_at_index(input_buffer, 1))
        {
            goto fail; /* nothing comes after the comma */
        }

        /* 解析子节点的键名 */
        input_buffer->offset++;
        // 跳过键名前的空白字符
        buffer_skip_whitespace(input_buffer);
        // 调用parse_string解析键名 键名必须是JSON字符串 失败则清理
        if (!parse_string(current_item, input_buffer))
        {
            goto fail; /* failed to parse name */
        }
        // 跳过键名后的空白字符
        buffer_skip_whitespace(input_buffer);

        /* 交换valuestring和string字段 因为当前解析的是键名
         * parse_string会把解析结果存入valuestring 而对象键名需要存在string字段
         */
        current_item->string = current_item->valuestring;
        current_item->valuestring = NULL;

        // 缓冲区无内容 或 当前字符不是冒号 键值对格式错误
        if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != ':'))
        {
            goto fail; /* invalid object */
        }

        /* 解析键对应的值 */
        input_buffer->offset++;
        // 跳过值前的空白字符
        buffer_skip_whitespace(input_buffer);
        // 调用parse_value解析值内容 支持任意JSON值类型 失败则清理
        if (!parse_value(current_item, input_buffer))
        {
            goto fail; /* failed to parse value */
        }
        // 跳过值后的空白字符
        buffer_skip_whitespace(input_buffer);
    }
    // 只要当前字符是逗号 就继续解析下一个键值对
    while (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ','));

    // 解析到缓冲区末尾 或 当前字符不是右大括号 对象格式错误
    if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != '}'))
    {
        goto fail; /* expected end of object */
    }

success:
    // 嵌套深度减1 完成当前对象解析
    input_buffer->depth--;

    // 链表不为空时 把尾节点的prev指向头节点 形成循环链表
    if (head != NULL) {
        head->prev = current_item;
    }

    // 设置当前节点为对象类型
    item->type = cJSON_Object;
    // 绑定对象的键值对子节点链表
    item->child = head;

    // 移动解析指针 跳过右大括号
    input_buffer->offset++;
    return true;

fail:
    // 解析失败 释放已分配的键值对链表
    if (head != NULL)
    {
        cJSON_Delete(head);
    }

    return false;
}

/* Render an object to text. */
/*
 * 函数名：print_object
 * 作用：将cJSON对象节点转换为JSON对象格式字符串
 *       是print_value的下层函数 递归处理对象中的每个键值对
 * 参数：
 *   item：待转换的cJSON对象节点
 *   output_buffer：输出缓冲区 存储拼接的字符串
 * 返回值：
 *   成功：返回true 缓冲区中已写入对象格式字符串
 *   失败：返回false 写入失败
 * 逻辑关联：
 *   上层调用：print_value → print_object
 *   下层调用：print_object → print_string_ptr 输出键名 → print_value 输出值
 *   生成规则：拼接{}包裹的逗号分隔键值对 格式化模式下加缩进 换行 制表符 紧凑模式无多余字符
 */
static cJSON_bool print_object(const cJSON * const item, printbuffer * const output_buffer)
{
    // 指向缓冲区当前写入位置的指针
    unsigned char *output_pointer = NULL;
    // 存储需要的缓冲区长度
    size_t length = 0;
    // 遍历对象的当前键值对节点
    cJSON *current_item = item->child;

    // 缓冲区为空 返回失败
    if (output_buffer == NULL)
    {
        return false;
    }

    /* 拼接输出对象字符串 */
    // 计算初始空间 格式化模式需要2字节 {+换行 紧凑模式1字节 仅{
    length = (size_t) (output_buffer->format ? 2 : 1); /* fmt: {\n */
    // 确保缓冲区有足够空间
    output_pointer = ensure(output_buffer, length + 1);
    if (output_pointer == NULL)
    {
        return false;
    }

    *output_pointer++ = '{';          // 写入左大括号
    output_buffer->depth++;           // 嵌套深度加1 用于格式化缩进
    if (output_buffer->format)
    {
        *output_pointer++ = '\n';     // 格式化模式下写入换行
    }
    output_buffer->offset += length;  // 更新缓冲区写入位置

    // 遍历对象的所有键值对
    while (current_item)
    {
        // 格式化模式下写入缩进 按嵌套深度添加制表符
        if (output_buffer->format)
        {
            size_t i;
            // 确保有足够空间存储缩进的制表符
            output_pointer = ensure(output_buffer, output_buffer->depth);
            if (output_pointer == NULL)
            {
                return false;
            }
            // 写入对应深度的制表符
            for (i = 0; i < output_buffer->depth; i++)
            {
                *output_pointer++ = '\t';
            }
            // 更新写入位置
            output_buffer->offset += output_buffer->depth;
        }

        /* 输出键名 */
        // 调用print_string_ptr把键名转为带双引号的字符串
        if (!print_string_ptr((unsigned char*)current_item->string, output_buffer))
        {
            return false;
        }
        // 更新缓冲区写入位置
        update_offset(output_buffer);

        // 计算冒号所需空间 格式化模式2字节 冒号+制表符 紧凑模式1字节 仅冒号
        length = (size_t) (output_buffer->format ? 2 : 1);
        output_pointer = ensure(output_buffer, length);
        if (output_pointer == NULL)
        {
            return false;
        }
        *output_pointer++ = ':';      // 写入冒号
        if (output_buffer->format)
        {
            *output_pointer++ = '\t'; // 格式化模式下加制表符
        }
        output_buffer->offset += length; // 更新写入位置

        /* 输出值 */
        // 调用print_value把值转为对应格式的字符串
        if (!print_value(current_item, output_buffer))
        {
            return false;
        }
        // 更新缓冲区写入位置
        update_offset(output_buffer);

        /* 非最后一个键值对则输出逗号 */
        // 计算所需空间 格式化模式加换行 有下一个元素加逗号
        length = ((size_t)(output_buffer->format ? 1 : 0) + (size_t)(current_item->next ? 1 : 0));
        output_pointer = ensure(output_buffer, length + 1);
        if (output_pointer == NULL)
        {
            return false;
        }
        // 有下一个元素则写入逗号
        if (current_item->next)
        {
            *output_pointer++ = ',';
        }

        // 格式化模式下写入换行
        if (output_buffer->format)
        {
            *output_pointer++ = '\n';
        }
        *output_pointer = '\0';       // 写入字符串结束符
        output_buffer->offset += length; // 更新写入位置

        // 移动到下一个键值对
        current_item = current_item->next;
    }

    // 计算写入右大括号所需空间 格式化模式需缩进+右大括号 紧凑模式仅右大括号
    output_pointer = ensure(output_buffer, output_buffer->format ? (output_buffer->depth + 1) : 2);
    if (output_pointer == NULL)
    {
        return false;
    }
    // 格式化模式下写入缩进 比当前深度少1级
    if (output_buffer->format)
    {
        size_t i;
        for (i = 0; i < (output_buffer->depth - 1); i++)
        {
            *output_pointer++ = '\t';
        }
    }
    *output_pointer++ = '}'; // 写入右大括号
    *output_pointer = '\0';  // 写入结束符
    output_buffer->depth--;  // 嵌套深度减1

    return true;
}

/* Get Array size/item / object item. */
/*
 * 函数名：cJSON_GetArraySize
 * 作用：对外暴露的获取cJSON数组长度的接口
 *       统计数组节点下的子元素个数 也可用于统计对象的键值对个数
 * 参数：
 *   array：待统计的cJSON数组或对象节点 传NULL返回0
 * 返回值：
 *   成功：返回数组/对象的元素个数 转换为int类型
 *   失败：返回0 如传入NULL或非数组对象节点
 * 逻辑关联：
 *   适用场景：用户遍历数组前获取长度 用于循环控制
 *   注意事项：数组长度超过int最大值时会溢出 是API设计缺陷 无法兼容修复
 *   实现逻辑：遍历数组/对象的子节点链表 统计节点个数
 */
CJSON_PUBLIC(int) cJSON_GetArraySize(const cJSON *array)
{
    // 遍历子节点的临时指针
    cJSON *child = NULL;
    // 统计元素个数 用size_t避免溢出 最后转换为int
    size_t size = 0;

    // 传入NULL直接返回0
    if (array == NULL)
    {
        return 0;
    }

    // 从第一个子节点开始遍历
    child = array->child;

    // 循环遍历所有子节点 统计个数
    while(child != NULL)
    {
        size++;
        child = child->next;
    }

    /* FIXME: Can overflow here. Cannot be fixed without breaking the API */
    // 注释说明：此处存在int溢出风险 但修改会破坏API兼容性 无法修复

    // 转换为int返回 适配旧版API
    return (int)size;
}
/*
 * 函数名：get_array_item
 * 作用：内部工具函数 按索引获取cJSON数组的指定元素
 *       是cJSON_GetArrayItem的底层实现 处理无符号索引的核心逻辑
 * 参数：
 *   array：待查询的cJSON数组节点 传NULL返回NULL
 *   index：要获取的元素索引 从0开始 无符号整数
 * 返回值：
 *   成功：返回对应索引的数组元素节点
 *   失败：返回NULL 如数组为空 索引超出范围
 * 逻辑关联：
 *   上层调用：cJSON_GetArrayItem → get_array_item
 *   实现逻辑：遍历数组子节点链表 按索引定位元素
 */
static cJSON* get_array_item(const cJSON *array, size_t index)
{
    // 遍历数组的临时节点指针
    cJSON *current_child = NULL;

    // 传入数组节点为NULL 直接返回失败
    if (array == NULL)
    {
        return NULL;
    }

    // 从数组第一个子节点开始遍历
    current_child = array->child;
    // 循环递减索引 直到找到目标位置或遍历完链表
    while ((current_child != NULL) && (index > 0))
    {
        index--;
        current_child = current_child->next;
    }

    // 返回目标节点 索引超出范围则返回NULL
    return current_child;
}

/*
 * 函数名：cJSON_GetArrayItem
 * 作用：对外暴露的按索引获取数组元素接口
 *       封装底层get_array_item 增加负数索引校验
 * 参数：
 *   array：待查询的cJSON数组节点 传NULL返回NULL
 *   index：要获取的元素索引 从0开始 传负数直接返回NULL
 * 返回值：
 *   成功：返回对应索引的数组元素节点
 *   失败：返回NULL 如数组为空 索引为负 索引超出范围
 * 逻辑关联：
 *   下层调用：cJSON_GetArrayItem → get_array_item
 *   注意事项：索引是int类型 超大数组可能溢出 但为了兼容API未修改
 */
CJSON_PUBLIC(cJSON *) cJSON_GetArrayItem(const cJSON *array, int index)
{
    // 负数索引非法 直接返回NULL
    if (index < 0)
    {
        return NULL;
    }

    // 转换为size_t类型 调用底层函数
    return get_array_item(array, (size_t)index);
}

/*
 * 函数名：get_object_item
 * 作用：内部工具函数 按键名获取cJSON对象的指定键值对
 *       支持区分/不区分大小写匹配键名 是对象访问接口的底层实现
 * 参数：
 *   object：待查询的cJSON对象节点 传NULL返回NULL
 *   name：要查询的键名 传NULL返回NULL
 *   case_sensitive：是否区分大小写 1区分 0不区分
 * 返回值：
 *   成功：返回对应键名的键值对节点
 *   失败：返回NULL 如对象为空 键名不存在
 * 逻辑关联：
 *   上层调用：cJSON_GetObjectItem → get_object_item（不区分大小写）
 *             cJSON_GetObjectItemCaseSensitive → get_object_item（区分大小写）
 *   实现逻辑：遍历对象子节点链表 匹配键名字符串
 */
static cJSON *get_object_item(const cJSON * const object, const char * const name, const cJSON_bool case_sensitive)
{
    // 遍历对象的临时节点指针
    cJSON *current_element = NULL;

    // 对象节点或键名为NULL 直接返回失败
    if ((object == NULL) || (name == NULL))
    {
        return NULL;
    }

    // 从对象第一个子节点开始遍历
    current_element = object->child;
    // 区分大小写匹配键名
    if (case_sensitive)
    {
        // 循环遍历 直到找到匹配的键名或遍历完链表
        while ((current_element != NULL) && (current_element->string != NULL) && (strcmp(name, current_element->string) != 0))
        {
            current_element = current_element->next;
        }
    }
    // 不区分大小写匹配键名
    else
    {
        // 使用大小写不敏感的字符串比较函数
        while ((current_element != NULL) && (case_insensitive_strcmp((const unsigned char*)name, (const unsigned char*)(current_element->string)) != 0))
        {
            current_element = current_element->next;
        }
    }

    // 校验找到的节点是否有效 键名字段不能为空
    if ((current_element == NULL) || (current_element->string == NULL)) {
        return NULL;
    }

    // 返回匹配的键值对节点
    return current_element;
}

/*
 * 函数名：cJSON_GetObjectItem
 * 作用：对外暴露的按键名获取对象值接口 不区分大小写
 *       封装底层get_object_item 适配大部分用户的使用习惯
 * 参数：
 *   object：待查询的cJSON对象节点 传NULL返回NULL
 *   string：要查询的键名 传NULL返回NULL
 * 返回值：
 *   成功：返回对应键名的键值对节点
 *   失败：返回NULL 如对象为空 键名不存在
 * 逻辑关联：
 *   下层调用：cJSON_GetObjectItem → get_object_item（case_sensitive=false）
 *   适用场景：不关心键名大小写的通用查询场景
 */
CJSON_PUBLIC(cJSON *) cJSON_GetObjectItem(const cJSON * const object, const char * const string)
{
    // 调用底层函数 不区分大小写匹配键名
    return get_object_item(object, string, false);
}

/*
 * 函数名：cJSON_GetObjectItemCaseSensitive
 * 作用：对外暴露的按键名获取对象值接口 严格区分大小写
 *       封装底层get_object_item 满足精准匹配的需求
 * 参数：
 *   object：待查询的cJSON对象节点 传NULL返回NULL
 *   string：要查询的键名 传NULL返回NULL
 * 返回值：
 *   成功：返回对应键名的键值对节点
 *   失败：返回NULL 如对象为空 键名不存在
 * 逻辑关联：
 *   下层调用：cJSON_GetObjectItemCaseSensitive → get_object_item（case_sensitive=true）
 *   适用场景：需要精准匹配键名大小写的场景 如JSON规范严格的接口
 */
CJSON_PUBLIC(cJSON *) cJSON_GetObjectItemCaseSensitive(const cJSON * const object, const char * const string)
{
    // 调用底层函数 严格区分大小写匹配键名
    return get_object_item(object, string, true);
}

/*
 * 函数名：cJSON_HasObjectItem
 * 作用：对外暴露的判断对象是否包含指定键名的接口
 *       基于cJSON_GetObjectItem实现 简化存在性判断
 * 参数：
 *   object：待查询的cJSON对象节点 传NULL返回false
 *   string：要查询的键名 传NULL返回false
 * 返回值：
 *   存在：返回1（true）
 *   不存在：返回0（false）
 * 逻辑关联：
 *   下层调用：cJSON_HasObjectItem → cJSON_GetObjectItem
 *   实现逻辑：通过查询结果是否为NULL 判断键名是否存在
 */
CJSON_PUBLIC(cJSON_bool) cJSON_HasObjectItem(const cJSON *object, const char *string)
{
    // 查询结果非空则返回1 否则返回0
    return cJSON_GetObjectItem(object, string) ? 1 : 0;
}

/* Utility for array list handling. */
/*
 * 函数名：suffix_object
 * 作用：内部工具函数 给cJSON链表节点追加下一个节点
 *       维护节点的prev和next指针 保证链表结构完整
 * 参数：
 *   prev：链表的前一个节点
 *   item：要追加的新节点
 * 返回值：无
 * 逻辑关联：
 *   上层调用：add_item_to_array → suffix_object
 *   实现逻辑：设置前节点的next和新节点的prev 完成链表拼接
 */
static void suffix_object(cJSON *prev, cJSON *item)
{
    prev->next = item;  // 前节点的下一个节点指向新节点
    item->prev = prev;  // 新节点的上一个节点指向前节点
}

/* Utility for handling references. */
/*
 * 函数名：create_reference
 * 作用：内部工具函数 创建cJSON节点的引用
 *       复制节点内容但不复制字符串 标记为引用类型 节省内存
 * 参数：
 *   item：要创建引用的源节点 传NULL返回NULL
 *   hooks：内存操作函数集合 用于分配新节点内存
 * 返回值：
 *   成功：返回引用节点 类型标记为cJSON_IsReference
 *   失败：返回NULL 如内存分配失败
 * 逻辑关联：
 *   上层调用：cJSON_AddItemReferenceToArray → create_reference
 *             cJSON_AddItemReferenceToObject → create_reference
 *   内存规则：引用节点不复制string字段 避免重复内存占用
 */
static cJSON *create_reference(const cJSON *item, const internal_hooks * const hooks)
{
    // 存储创建的引用节点
    cJSON *reference = NULL;
    // 源节点为空 直接返回失败
    if (item == NULL)
    {
        return NULL;
    }

    // 分配新节点内存
    reference = cJSON_New_Item(hooks);
    if (reference == NULL)
    {
        return NULL;
    }

    // 复制源节点的所有内容
    memcpy(reference, item, sizeof(cJSON));
    reference->string = NULL;                // 引用节点不持有字符串
    reference->type |= cJSON_IsReference;    // 标记为引用类型
    reference->next = reference->prev = NULL;// 重置链表指针
    return reference;
}

/*
 * 函数名：add_item_to_array
 * 作用：内部工具函数 给cJSON数组添加元素节点
 *       维护数组的链表结构 支持空数组和非空数组的追加逻辑
 * 参数：
 *   array：目标数组节点 传NULL返回false
 *   item：要添加的元素节点 传NULL或与数组相同返回false
 * 返回值：
 *   成功：返回true
 *   失败：返回false 如参数非法
 * 逻辑关联：
 *   上层调用：cJSON_AddItemToArray → add_item_to_array
 *             add_item_to_object → add_item_to_array
 *   实现逻辑：空数组直接设为第一个节点 非空数组追加到链表尾部
 */
static cJSON_bool add_item_to_array(cJSON *array, cJSON *item)
{
    // 数组子节点的临时指针
    cJSON *child = NULL;

    // 参数合法性校验 节点为空或自引用 返回失败
    if ((item == NULL) || (array == NULL) || (array == item))
    {
        return false;
    }

    child = array->child;
    /* 利用array的prev指针快速找到最后一个节点 提升追加效率 */
    // 数组为空 初始化链表
    if (child == NULL)
    {
        /* list is empty, start new one */
        array->child = item;       // 设为第一个子节点
        item->prev = item;         // 尾节点的prev指向自身 标记链表尾部
        item->next = NULL;
    }
    else
    {
        /* 追加到链表尾部 */
        if (child->prev)
        {
            suffix_object(child->prev, item); // 追加新节点到尾部
            array->child->prev = item;        // 更新尾节点标记
        }
    }

    return true;
}

/* Add item to array/object. */
/*
 * 函数名：cJSON_AddItemToArray
 * 作用：对外暴露的给数组添加元素的接口
 *       直接封装底层add_item_to_array 简化用户调用
 * 参数：
 *   array：目标数组节点 传NULL返回false
 *   item：要添加的元素节点 传NULL返回false
 * 返回值：
 *   成功：返回true
 *   失败：返回false 如参数非法
 * 逻辑关联：
 *   下层调用：cJSON_AddItemToArray → add_item_to_array
 *   注意事项：添加后节点所有权归数组 无需用户手动管理
 */
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToArray(cJSON *array, cJSON *item)
{
    return add_item_to_array(array, item);
}

// 编译器兼容处理 暂时关闭GCC/Clang的类型限定符转换警告
#if defined(__clang__) || (defined(__GNUC__)  && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 5))))
    #pragma GCC diagnostic push
#endif
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
/* helper function to cast away const */
/*
 * 函数名：cast_away_const
 * 作用：内部工具函数 移除指针的const限定符
 *       用于常量键名的场景 避免编译器警告
 * 参数：
 *   string：带const限定的指针
 * 返回值：移除const后的void指针
 * 注意事项：仅用于确认不会修改内容的场景 避免滥用
 */
static void* cast_away_const(const void* string)
{
    return (void*)string;
}
// 恢复编译器警告设置
#if defined(__clang__) || (defined(__GNUC__)  && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 5))))
    #pragma GCC diagnostic pop
#endif

/*
 * 函数名：add_item_to_object
 * 作用：内部工具函数 给cJSON对象添加键值对节点
 *       处理键名的内存管理 支持常量键名和动态键名
 * 参数：
 *   object：目标对象节点 传NULL返回false
 *   string：键名字符串 传NULL返回false
 *   item：要添加的键值对节点 传NULL返回false
 *   hooks：内存操作函数集合 用于键名的内存分配
 *   constant_key：是否为常量键名 1是 0否
 * 返回值：
 *   成功：返回true
 *   失败：返回false 如参数非法 内存分配失败
 * 逻辑关联：
 *   上层调用：cJSON_AddItemToObject → add_item_to_object
 *             cJSON_AddItemToObjectCS → add_item_to_object
 *   下层调用：add_item_to_object → add_item_to_array
 *   实现逻辑：处理键名内存 → 设置节点属性 → 调用数组添加函数（对象底层用链表存储）
 */
static cJSON_bool add_item_to_object(cJSON * const object, const char * const string, cJSON * const item, const internal_hooks * const hooks, const cJSON_bool constant_key)
{
    // 存储处理后的键名字符串/*
 * cJSON核心函数注释总结
 * 本注释覆盖cJSON从JSON解析、数据操作到字符串生成的全核心流程，核心逻辑如下：
 * 1. 解析流程：以parse_value为核心分支函数，依次匹配null/布尔/字符串/数字/数组/对象类型，
 *    其中parse_array递归解析数组元素、parse_object解析对象键值对，通过嵌套深度检查防止栈溢出，
 *    解析过程维护缓冲区指针和链表结构，失败时自动释放已分配内存避免泄漏；
 * 2. 生成流程：以print_value为核心分支函数，根据节点类型拼接对应JSON字符串，
 *    print_array/print_object分别处理数组/对象的格式化/紧凑输出，支持缩进、换行等格式控制，
 *    缓冲区自动扩容并适配用户预分配内存场景，最终返回符合JSON规范的字符串；
 * 3. 数据操作：提供数组/对象的核心访问接口（cJSON_GetArrayItem/cJSON_GetObjectItem），
 *    支持按索引/键名查询、存在性判断，对象查询可区分/不区分大小写；
 *    节点添加接口（cJSON_AddItemToArray/ToObject）维护链表prev/next指针，
 *    支持常量/动态键名、节点引用（节省内存），并提供各类数据类型的快捷添加接口；
 * 4. 内存管理：统一通过global_hooks管理内存分配/释放，解析/生成失败时自动清理内存，
 *    引用节点不复制字符串、常量键名不分配新内存，兼顾性能与内存安全；
 * 整体逻辑遵循JSON规范，上层对外暴露简洁API，下层通过内部工具函数实现核心逻辑，
 * 兼顾兼容性（如int类型溢出提示、编译器警告处理）与易用性，是cJSON轻量化、高性能的核心体现。
 */
    char *new_key = NULL;
    // 存储节点的新类型
    int new_type = cJSON_Invalid;

    // 参数合法性校验 任意关键参数为空或自引用 返回失败
    if ((object == NULL) || (string == NULL) || (item == NULL) || (object == item))
    {
        return false;
    }

    // 常量键名处理 直接转换指针 不复制内存
    if (constant_key)
    {
        new_key = (char*)cast_away_const(string);
        new_type = item->type | cJSON_StringIsConst; // 标记键名为常量
    }
    else
    {
        // 动态键名 复制字符串到新内存
        new_key = (char*)cJSON_strdup((const unsigned char*)string, hooks);
        if (new_key == NULL)
        {
            return false; // 内存分配失败
        }

        new_type = item->type & ~cJSON_StringIsConst; // 标记键名为动态分配
    }

    // 释放节点原有非常量键名 避免内存泄漏
    if (!(item->type & cJSON_StringIsConst) && (item->string != NULL))
    {
        hooks->deallocate(item->string);
    }

    // 设置新的键名和类型
    item->string = new_key;
    item->type = new_type;

    // 调用数组添加函数 对象底层用链表存储键值对
    return add_item_to_array(object, item);
}

/*
 * 函数名：cJSON_AddItemToObject
 * 作用：对外暴露的给对象添加键值对的接口
 *       封装底层add_item_to_object 动态分配键名内存
 * 参数：
 *   object：目标对象节点 传NULL返回false
 *   string：键名字符串 传NULL返回false
 *   item：要添加的键值对节点 传NULL返回false
 * 返回值：
 *   成功：返回true
 *   失败：返回false 如参数非法 内存分配失败
 * 逻辑关联：
 *   下层调用：cJSON_AddItemToObject → add_item_to_object（constant_key=false）
 *   内存规则：键名会被复制 原字符串可自由释放
 */
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item)
{
    // 调用底层函数 使用全局内存钩子 动态键名
    return add_item_to_object(object, string, item, &global_hooks, false);
}

/* Add an item to an object with constant string as key */
/*
 * 函数名：cJSON_AddItemToObjectCS
 * 作用：对外暴露的给对象添加常量键名键值对的接口
 *       封装底层add_item_to_object 不复制键名内存 节省开销
 * 参数：
 *   object：目标对象节点 传NULL返回false
 *   string：常量键名字符串 传NULL返回false
 *   item：要添加的键值对节点 传NULL返回false
 * 返回值：
 *   成功：返回true
 *   失败：返回false 如参数非法
 * 逻辑关联：
 *   下层调用：cJSON_AddItemToObjectCS → add_item_to_object（constant_key=true）
 *   注意事项：常量键名的生命周期需长于对象节点 避免野指针
 */
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToObjectCS(cJSON *object, const char *string, cJSON *item)
{
    // 调用底层函数 使用全局内存钩子 常量键名
    return add_item_to_object(object, string, item, &global_hooks, true);
}

/*
 * 函数名：cJSON_AddItemReferenceToArray
 * 作用：对外暴露的给数组添加节点引用的接口
 *       创建源节点的引用 而非复制 节省内存
 * 参数：
 *   array：目标数组节点 传NULL返回false
 *   item：要添加引用的源节点 传NULL返回false
 * 返回值：
 *   成功：返回true
 *   失败：返回false 如参数非法 引用创建失败
 * 逻辑关联：
 *   下层调用：cJSON_AddItemReferenceToArray → create_reference → add_item_to_array
 *   适用场景：大节点复用 避免重复复制内存
 */
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item)
{
    // 数组节点为空 直接返回失败
    if (array == NULL)
    {
        return false;
    }

    // 创建引用节点并添加到数组
    return add_item_to_array(array, create_reference(item, &global_hooks));
}

/*
 * 函数名：cJSON_AddItemReferenceToObject
 * 作用：对外暴露的给对象添加节点引用的接口
 *       创建源节点的引用 并绑定键名 节省内存
 * 参数：
 *   object：目标对象节点 传NULL返回false
 *   string：键名字符串 传NULL返回false
 *   item：要添加引用的源节点 传NULL返回false
 * 返回值：
 *   成功：返回true
 *   失败：返回false 如参数非法 引用创建失败 内存分配失败
 * 逻辑关联：
 *   下层调用：cJSON_AddItemReferenceToObject → create_reference → add_item_to_object
 *   适用场景：大节点复用 避免重复复制内存
 */
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemReferenceToObject(cJSON *object, const char *string, cJSON *item)
{
    // 对象节点或键名为NULL 直接返回失败
    if ((object == NULL) || (string == NULL))
    {
        return false;
    }

    // 创建引用节点并添加到对象
    return add_item_to_object(object, string, create_reference(item, &global_hooks), &global_hooks, false);
}

/*
 * 函数名：cJSON_AddNullToObject
 * 作用：对外暴露的给对象添加NULL类型键值对的快捷接口
 *       简化创建NULL节点+添加到对象的流程
 * 参数：
 *   object：目标对象节点 传NULL返回NULL
 *   name：键名字符串 传NULL返回NULL
 * 返回值：
 *   成功：返回创建的NULL节点
 *   失败：返回NULL 如对象为空 内存分配失败 添加失败
 * 逻辑关联：
 *   下层调用：cJSON_AddNullToObject → cJSON_CreateNull → add_item_to_object
 *   内存规则：创建失败会自动释放节点 避免内存泄漏
 */
CJSON_PUBLIC(cJSON*) cJSON_AddNullToObject(cJSON * const object, const char * const name)
{
    // 创建NULL类型节点
    cJSON *null = cJSON_CreateNull();
    // 添加到对象成功则返回节点
    if (add_item_to_object(object, name, null, &global_hooks, false))
    {
        return null;
    }

    // 添加失败 释放创建的节点
    cJSON_Delete(null);
    return NULL;
}

/*
 * 函数名：cJSON_AddTrueToObject
 * 作用：对外暴露的给对象添加true类型键值对的快捷接口
 *       简化创建true节点+添加到对象的流程
 * 参数：
 *   object：目标对象节点 传NULL返回NULL
 *   name：键名字符串 传NULL返回NULL
 * 返回值：
 *   成功：返回创建的true节点
 *   失败：返回NULL 如对象为空 内存分配失败 添加失败
 * 逻辑关联：
 *   下层调用：cJSON_AddTrueToObject → cJSON_CreateTrue → add_item_to_object
 *   内存规则：创建失败会自动释放节点 避免内存泄漏
 */
CJSON_PUBLIC(cJSON*) cJSON_AddTrueToObject(cJSON * const object, const char * const name)
{
    // 创建true类型节点
    cJSON *true_item = cJSON_CreateTrue();
    // 添加到对象成功则返回节点
    if (add_item_to_object(object, name, true_item, &global_hooks, false))
    {
        return true_item;
    }

    // 添加失败 释放创建的节点
    cJSON_Delete(true_item);
    return NULL;
}

/*
 * 函数名：cJSON_AddFalseToObject
 * 作用：对外暴露的给对象添加false类型键值对的快捷接口
 *       简化创建false节点+添加到对象的流程
 * 参数：
 *   object：目标对象节点 传NULL返回NULL
 *   name：键名字符串 传NULL返回NULL
 * 返回值：
 *   成功：返回创建的false节点
 *   失败：返回NULL 如对象为空 内存分配失败 添加失败
 * 逻辑关联：
 *   下层调用：cJSON_AddFalseToObject → cJSON_CreateFalse → add_item_to_object
 *   内存规则：创建失败会自动释放节点 避免内存泄漏
 */
CJSON_PUBLIC(cJSON*) cJSON_AddFalseToObject(cJSON * const object, const char * const name)
{
    // 创建false类型节点
    cJSON *false_item = cJSON_CreateFalse();
    // 添加到对象成功则返回节点
    if (add_item_to_object(object, name, false_item, &global_hooks, false))
    {
        return false_item;
    }

    // 添加失败 释放创建的节点
    cJSON_Delete(false_item);
    return NULL;
}

/*
 * 函数名：cJSON_AddBoolToObject
 * 作用：对外暴露的给对象添加布尔类型键值对的快捷接口
 *       根据传入的布尔值创建true/false节点 简化调用
 * 参数：
 *   object：目标对象节点 传NULL返回NULL
 *   name：键名字符串 传NULL返回NULL
 *   boolean：布尔值 非0为true 0为false
 * 返回值：
 *   成功：返回创建的布尔节点
 *   失败：返回NULL 如对象为空 内存分配失败 添加失败
 * 逻辑关联：
 *   下层调用：cJSON_AddBoolToObject → cJSON_CreateBool → add_item_to_object
 *   内存规则：创建失败会自动释放节点 避免内存泄漏
 */
CJSON_PUBLIC(cJSON*) cJSON_AddBoolToObject(cJSON * const object, const char * const name, const cJSON_bool boolean)
{
    // 根据布尔值创建对应节点
    cJSON *bool_item = cJSON_CreateBool(boolean);
    // 添加到对象成功则返回节点
    if (add_item_to_object(object, name, bool_item, &global_hooks, false))
    {
        return bool_item;
    }

    // 添加失败 释放创建的节点
    cJSON_Delete(bool_item);
    return NULL;
}

/*
 * 函数名：cJSON_AddNumberToObject
 * 作用：对外暴露的给对象添加数字类型键值对的快捷接口
 *       简化创建数字节点+添加到对象的流程
 * 参数：
 *   object：目标对象节点 传NULL返回NULL
 *   name：键名字符串 传NULL返回NULL
 *   number：要添加的数字值 双精度浮点型
 * 返回值：
 *   成功：返回创建的数字节点
 *   失败：返回NULL 如对象为空 内存分配失败 添加失败
 * 逻辑关联：
 *   下层调用：cJSON_AddNumberToObject → cJSON_CreateNumber → add_item_to_object
 *   内存规则：创建失败会自动释放节点 避免内存泄漏
 */
CJSON_PUBLIC(cJSON*) cJSON_AddNumberToObject(cJSON * const object, const char * const name, const double number)
{
    // 创建数字类型节点
    cJSON *number_item = cJSON_CreateNumber(number);
    // 添加到对象成功则返回节点
    if (add_item_to_object(object, name, number_item, &global_hooks, false))
    {
        return number_item;
    }

    // 添加失败 释放创建的节点
    cJSON_Delete(number_item);
    return NULL;
}

/*
 * 函数名：cJSON_AddStringToObject
 * 作用：对外暴露的给对象添加字符串类型键值对的快捷接口
 *       简化创建字符串节点+添加到对象的流程
 * 参数：
 *   object：目标对象节点 传NULL返回NULL
 *   name：键名字符串 传NULL返回NULL
 *   string：要添加的字符串值 传NULL创建空字符串节点
 * 返回值：
 *   成功：返回创建的字符串节点
 *   失败：返回NULL 如对象为空 内存分配失败 添加失败
 * 逻辑关联：
 *   下层调用：cJSON_AddStringToObject → cJSON_CreateString → add_item_to_object
 *   内存规则：创建失败会自动释放节点 避免内存泄漏
 */
CJSON_PUBLIC(cJSON*) cJSON_AddStringToObject(cJSON * const object, const char * const name, const char * const string)
{
    // 创建字符串类型节点
    cJSON *string_item = cJSON_CreateString(string);
    // 添加到对象成功则返回节点
    if (add_item_to_object(object, name, string_item, &global_hooks, false))
    {
        return string_item;
    }

    // 添加失败 释放创建的节点
    cJSON_Delete(string_item);
    return NULL;
}

/*
 * 函数名：cJSON_AddRawToObject
 * 作用：对外暴露的给对象添加原始JSON字符串类型键值对的快捷接口
 *       简化创建原始节点+添加到对象的流程 不转义字符串
 * 参数：
 *   object：目标对象节点 传NULL返回NULL
 *   name：键名字符串 传NULL返回NULL
 *   raw：要添加的原始JSON字符串 传NULL返回NULL
 * 返回值：
 *   成功：返回创建的原始节点
 *   失败：返回NULL 如对象为空 内存分配失败 添加失败
 * 逻辑关联：
 *   下层调用：cJSON_AddRawToObject → cJSON_CreateRaw → add_item_to_object
 *   适用场景：直接插入预格式化的JSON字符串 避免二次解析
 *   内存规则：创建失败会自动释放节点 避免内存泄漏
 */
CJSON_PUBLIC(cJSON*) cJSON_AddRawToObject(cJSON * const object, const char * const name, const char * const raw)
{
    // 创建原始JSON类型节点
    cJSON *raw_item = cJSON_CreateRaw(raw);
    // 添加到对象成功则返回节点
    if (add_item_to_object(object, name, raw_item, &global_hooks, false))
    {
        return raw_item;
    }

    // 添加失败 释放创建的节点
    cJSON_Delete(raw_item);
    return NULL;
}

/*
 * 函数名：cJSON_AddObjectToObject
 * 作用：对外暴露的给对象添加子对象类型键值对的快捷接口
 *       简化创建空对象节点+添加到对象的流程
 * 参数：
 *   object：目标对象节点 传NULL返回NULL
 *   name：键名字符串 传NULL返回NULL
 * 返回值：
 *   成功：返回创建的空对象节点
 *   失败：返回NULL 如对象为空 内存分配失败 添加失败
 * 逻辑关联：
 *   下层调用：cJSON_AddObjectToObject → cJSON_CreateObject → add_item_to_object
 *   内存规则：创建失败会自动释放节点 避免内存泄漏
 */
CJSON_PUBLIC(cJSON*) cJSON_AddObjectToObject(cJSON * const object, const char * const name)
{
    // 创建空对象类型节点
    cJSON *object_item = cJSON_CreateObject();
    // 添加到对象成功则返回节点
    if (add_item_to_object(object, name, object_item, &global_hooks, false))
    {
        return object_item;
    }

    // 添加失败 释放创建的节点
    cJSON_Delete(object_item);
    return NULL;
}
/*
 * cJSON核心函数注释总结
 * 1. 解析流程：以parse_value为核心分支函数，依次匹配null/布尔/字符串/数字/数组/对象类型，
 *    其中parse_array递归解析数组元素、parse_object解析对象键值对，通过嵌套深度检查防止栈溢出，
 *    解析过程维护缓冲区指针和链表结构，失败时自动释放已分配内存避免泄漏；
 * 2. 生成流程：以print_value为核心分支函数，根据节点类型拼接对应JSON字符串，
 *    print_array/print_object分别处理数组/对象的格式化/紧凑输出，支持缩进、换行等格式控制，
 *    缓冲区自动扩容并适配用户预分配内存场景，最终返回符合JSON规范的字符串；
 * 3. 数据操作：提供数组/对象的核心访问接口（cJSON_GetArrayItem/cJSON_GetObjectItem），
 *    支持按索引/键名查询、存在性判断，对象查询可区分/不区分大小写；
 *    节点添加接口（cJSON_AddItemToArray/ToObject）维护链表prev/next指针，
 *    支持常量/动态键名、节点引用（节省内存），并提供各类数据类型的快捷添加接口；
 * 4. 内存管理：统一通过global_hooks管理内存分配/释放，解析/生成失败时自动清理内存，
 *    引用节点不复制字符串、常量键名不分配新内存，兼顾性能与内存安全；
 */
CJSON_PUBLIC(cJSON*) cJSON_AddArrayToObject(cJSON * const object, const char * const name)
{
    cJSON *array = cJSON_CreateArray();
    if (add_item_to_object(object, name, array, &global_hooks, false))
    {
        return array;
    }

    cJSON_Delete(array);
    return NULL;
}

CJSON_PUBLIC(cJSON *) cJSON_DetachItemViaPointer(cJSON *parent, cJSON * const item)
{
    if ((parent == NULL) || (item == NULL) || (item != parent->child && item->prev == NULL))
    {
        return NULL;
    }

    if (item != parent->child)
    {
        /* not the first element */
        item->prev->next = item->next;
    }
    if (item->next != NULL)
    {
        /* not the last element */
        item->next->prev = item->prev;
    }

    if (item == parent->child)
    {
        /* first element */
        parent->child = item->next;
    }
    else if (item->next == NULL)
    {
        /* last element */
        parent->child->prev = item->prev;
    }

    /* make sure the detached item doesn't point anywhere anymore */
    item->prev = NULL;
    item->next = NULL;

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromArray(cJSON *array, int which)
{
    if (which < 0)
    {
        return NULL;
    }

    return cJSON_DetachItemViaPointer(array, get_array_item(array, (size_t)which));
}

CJSON_PUBLIC(void) cJSON_DeleteItemFromArray(cJSON *array, int which)
{
    cJSON_Delete(cJSON_DetachItemFromArray(array, which));
}

CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromObject(cJSON *object, const char *string)
{
    cJSON *to_detach = cJSON_GetObjectItem(object, string);

    return cJSON_DetachItemViaPointer(object, to_detach);
}

CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromObjectCaseSensitive(cJSON *object, const char *string)
{
    cJSON *to_detach = cJSON_GetObjectItemCaseSensitive(object, string);

    return cJSON_DetachItemViaPointer(object, to_detach);
}

CJSON_PUBLIC(void) cJSON_DeleteItemFromObject(cJSON *object, const char *string)
{
    cJSON_Delete(cJSON_DetachItemFromObject(object, string));
}

CJSON_PUBLIC(void) cJSON_DeleteItemFromObjectCaseSensitive(cJSON *object, const char *string)
{
    cJSON_Delete(cJSON_DetachItemFromObjectCaseSensitive(object, string));
}
与系统头文件中的单行注释有关
/* Replace array/object items with new ones. */
CJSON_PUBLIC(cJSON_bool) cJSON_InsertItemInArray(cJSON *array, int which, cJSON *newitem)
{
    cJSON *after_inserted = NULL;

    if (which < 0 || newitem == NULL)
    {
        return false;
    }

    after_inserted = get_array_item(array, (size_t)which);
    if (after_inserted == NULL)
    {
        return add_item_to_array(array, newitem);
    }

    if (after_inserted != array->child && after_inserted->prev == NULL) {
        /* return false if after_inserted is a corrupted array item */
        return false;
    }

    newitem->next = after_inserted;
    newitem->prev = after_inserted->prev;
    after_inserted->prev = newitem;
    if (after_inserted == array->child)
    {
        array->child = newitem;
    }
    else
    {
        newitem->prev->next = newitem;
    }
    return true;
}

CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemViaPointer(cJSON * const parent, cJSON * const item, cJSON * replacement)
{
    if ((parent == NULL) || (parent->child == NULL) || (replacement == NULL) || (item == NULL))
    {
        return false;
    }

    if (replacement == item)
    {
        return true;
    }

    replacement->next = item->next;
    replacement->prev = item->prev;

    if (replacement->next != NULL)
    {
        replacement->next->prev = replacement;
    }
    if (parent->child == item)
    {
        if (parent->child->prev == parent->child)
        {
            replacement->prev = replacement;
        }
        parent->child = replacement;
    }
    else
    {   /*
         * To find the last item in array quickly, we use prev in array.
         * We can't modify the last item's next pointer where this item was the parent's child
         */
        if (replacement->prev != NULL)
        {
            replacement->prev->next = replacement;
        }
        if (replacement->next == NULL)
        {
            parent->child->prev = replacement;
        }
    }

    item->next = NULL;
    item->prev = NULL;
    cJSON_Delete(item);

    return true;
}

CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemInArray(cJSON *array, int which, cJSON *newitem)
{
    if (which < 0)
    {
        return false;
    }

    return cJSON_ReplaceItemViaPointer(array, get_array_item(array, (size_t)which), newitem);
}

static cJSON_bool replace_item_in_object(cJSON *object, const char *string, cJSON *replacement, cJSON_bool case_sensitive)
{
    if ((replacement == NULL) || (string == NULL))
    {
        return false;
    }

    /* replace the name in the replacement */
    if (!(replacement->type & cJSON_StringIsConst) && (replacement->string != NULL))
    {
        cJSON_free(replacement->string);
    }
    replacement->string = (char*)cJSON_strdup((const unsigned char*)string, &global_hooks);
    if (replacement->string == NULL)
    {
        return false;
    }

    replacement->type &= ~cJSON_StringIsConst;

    return cJSON_ReplaceItemViaPointer(object, get_object_item(object, string, case_sensitive), replacement);
}

CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemInObject(cJSON *object, const char *string, cJSON *newitem)
{
    return replace_item_in_object(object, string, newitem, false);
}

CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemInObjectCaseSensitive(cJSON *object, const char *string, cJSON *newitem)
{
    return replace_item_in_object(object, string, newitem, true);
}

/* Create basic types: */
CJSON_PUBLIC(cJSON *) cJSON_CreateNull(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_NULL;
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateTrue(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_True;
    }
与系统头文件中的单行注释有关
    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateFalse(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_False;
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateBool(cJSON_bool boolean)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = boolean ? cJSON_True : cJSON_False;
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateNumber(double num)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_Number;
        item->valuedouble = num;

        /* use saturation in case of overflow */
        if (num >= INT_MAX)
        {
            item->valueint = INT_MAX;
        }
        else if (num <= (double)INT_MIN)
        {
            item->valueint = INT_MIN;
        }
        else
        {
            item->valueint = (int)num;
        }
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateString(const char *string)
{与系统头文件中的单行注释有关
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_String;
        item->valuestring = (char*)cJSON_strdup((const unsigned char*)string, &global_hooks);
        if(!item->valuestring)
        {
            cJSON_Delete(item);
            return NULL;
        }
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateStringReference(const char *string)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if (item != NULL)
    {
        item->type = cJSON_String | cJSON_IsReference;
        item->valuestring = (char*)cast_away_const(string);
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateObjectReference(const cJSON *child)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if (item != NULL) {
        item->type = cJSON_Object | cJSON_IsReference;
        item->child = (cJSON*)cast_away_const(child);
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateArrayReference(const cJSON *child) {
    cJSON *item = cJSON_New_Item(&global_hooks);
    if (item != NULL) {
        item->type = cJSON_Array | cJSON_IsReference;与系统头文件中的单行注释有关
        item->child = (cJSON*)cast_away_const(child);
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateRaw(const char *raw)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_Raw;
        item->valuestring = (char*)cJSON_strdup((const unsigned char*)raw, &global_hooks);
        if(!item->valuestring)
        {
            cJSON_Delete(item);
            return NULL;
        }
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateArray(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type=cJSON_Array;
    }

    return item;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateObject(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if (item)
    {
        item->type = cJSON_Object;
    }

    return item;
}

/* Create Arrays: */
CJSON_PUBLIC(cJSON *) cJSON_CreateIntArray(const int *numbers, int count)
{
    size_t i = 0;
    cJSON *n = NULL;
    cJSON *p = NULL;
    cJSON *a = NULL;

    if ((count < 0) || (numbers == NULL))
    {
        return NULL;
    }

    a = cJSON_CreateArray();

    for(i = 0; a && (i < (size_t)count); i++)
    {
        n = cJSON_CreateNumber(numbers[i]);
        if (!n)
        {
            cJSON_Delete(a);
            return NULL;
        }
        if(!i)
        {
            a->child = n;
        }
        else
        {
            suffix_object(p, n);
        }
        p = n;
    }

    if (a && a->child) {
        a->child->prev = n;
    }

    return a;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateFloatArray(const float *numbers, int count)
{
    size_t i = 0;
    cJSON *n = NULL;
    cJSON *p = NULL;
    cJSON *a = NULL;

    if ((count < 0) || (numbers == NULL))
    {
        return NULL;
    }

    a = cJSON_CreateArray();

    for(i = 0; a && (i < (size_t)count); i++)
    {
        n = cJSON_CreateNumber((double)numbers[i]);
        if(!n)
        {
            cJSON_Delete(a);
            return NULL;
        }
        if(!i)
        {
            a->child = n;
        }
        else
        {
            suffix_object(p, n);
        }
        p = n;
    }

    if (a && a->child) {
        a->child->prev = n;
    }

    return a;
}
与系统头文件中的单行注释有关
CJSON_PUBLIC(cJSON *) cJSON_CreateDoubleArray(const double *numbers, int count)
{
    size_t i = 0;
    cJSON *n = NULL;
    cJSON *p = NULL;
    cJSON *a = NULL;

    if ((count < 0) || (numbers == NULL))
    {
        return NULL;
    }

    a = cJSON_CreateArray();

    for(i = 0; a && (i < (size_t)count); i++)
    {
        n = cJSON_CreateNumber(numbers[i]);
        if(!n)
        {
            cJSON_Delete(a);
            return NULL;
        }
        if(!i)
        {
            a->child = n;
        }
        else
        {
            suffix_object(p, n);
        }
        p = n;
    }

    if (a && a->child) {
        a->child->prev = n;
    }

    return a;
}

CJSON_PUBLIC(cJSON *) cJSON_CreateStringArray(const char *const *strings, int count)
{
    size_t i = 0;
    cJSON *n = NULL;
    cJSON *p = NULL;
    cJSON *a = NULL;

    if ((count < 0) || (strings == NULL))
    {
        return NULL;
    }

    a = cJSON_CreateArray();

    for (i = 0; a && (i < (size_t)count); i++)
    {
        n = cJSON_CreateString(strings[i]);
        if(!n)
        {
            cJSON_Delete(a);
            return NULL;
        }
        if(!i)
        {
            a->child = n;
        }
        else
        {
            suffix_object(p,n);
        }
        p = n;
    }

    if (a && a->child) {
        a->child->prev = n;
    }

    return a;
}

/* Duplication */
cJSON * cJSON_Duplicate_rec(const cJSON *item, size_t depth, cJSON_bool recurse);

CJSON_PUBLIC(cJSON *) cJSON_Duplicate(const cJSON *item, cJSON_bool recurse)
{
    return cJSON_Duplicate_rec(item, 0, recurse );
}

cJSON * cJSON_Duplicate_rec(const cJSON *item, size_t depth, cJSON_bool recurse)
{
    cJSON *newitem = NULL;
    cJSON *child = NULL;
    cJSON *next = NULL;
    cJSON *newchild = NULL;

    /* Bail on bad ptr */
    if (!item)
    {
        goto fail;
    }
    /* Create new item */
    newitem = cJSON_New_Item(&global_hooks);
    if (!newitem)
    {
        goto fail;
    }
    /* Copy over all vars */
    newitem->type = item->type & (~cJSON_IsReference);
    newitem->valueint = item->valueint;
    newitem->valuedouble = item->valuedouble;
    if (item->valuestring)
    {
        newitem->valuestring = (char*)cJSON_strdup((unsigned char*)item->valuestring, &global_hooks);
        if (!newitem->valuestring)
        {
            goto fail;
        }
    }
    if (item->string)
    {
        newitem->string = (item->type&cJSON_StringIsConst) ? item->string : (char*)cJSON_strdup((unsigned char*)item->string, &global_hooks);
        if (!newitem->string)
        {
            goto fail;
        }
    }
    /* If non-recursive, then we're done! */
    if (!recurse)
    {
        return newitem;
    }
    /* Walk the ->next chain for the child. */
    child = item->child;
    while (child != NULL)
    {
        if(depth >= CJSON_CIRCULAR_LIMIT) {
            goto fail;
        }
        newchild = cJSON_Duplicate_rec(child, depth + 1, true); /* Duplicate (with recurse) each item in the ->next chain */
        if (!newchild)
        {
            goto fail;
        }
        if (next != NULL)
        {
            /* If newitem->child already set, then crosswire ->prev and ->next and move on */
            next->next = newchild;与系统头文件中的单行注释有关
            newchild->prev = next;
            next = newchild;
        }
        else
        {
            /* Set newitem->child and move to it */
            newitem->child = newchild;
            next = newchild;
        }
        child = child->next;
    }
    if (newitem && newitem->child)
    {
        newitem->child->prev = newchild;
    }

    return newitem;

fail:
    if (newitem != NULL)
    {
        cJSON_Delete(newitem);
    }

    return NULL;
}

static void skip_oneline_comment(char **input)
{
    *input += static_strlen("//");

    for (; (*input)[0] != '\0'; ++(*input))
    {
        if ((*input)[0] == '\n') {
            *input += static_strlen("\n");
            return;
        }
    }
}

static void skip_multiline_comment(char **input)
{
    *input += static_strlen("/*");

    for (; (*input)[0] != '\0'; ++(*input))
    {
        if (((*input)[0] == '*') && ((*input)[1] == '/'))
        {
            *input += static_strlen("*/");
            return;
        }
    }
}

static void minify_string(char **input, char **output) {
    (*output)[0] = (*input)[0];
    *input += static_strlen("\"");
    *output += static_strlen("\"");


    for (; (*input)[0] != '\0'; (void)++(*input), ++(*output)) {
        (*output)[0] = (*input)[0];

        if ((*input)[0] == '\"') {
            (*output)[0] = '\"';
            *input += static_strlen("\"");
            *output += static_strlen("\"");
            return;
        } else if (((*input)[0] == '\\') && ((*input)[1] == '\"')) {
            (*output)[1] = (*input)[1];
            *input += static_strlen("\"");
            *output += static_strlen("\"");
        }
    }
}

CJSON_PUBLIC(void) cJSON_Minify(char *json)
{
    char *into = json;

    if (json == NULL)
    {
        return;
    }

    while (json[0] != '\0')
    {
        switch (json[0])
        {
            case ' ':
            case '\t':与系统头文件中的单行注释有关
            case '\r':
            case '\n':
                json++;
                break;

            case '/':
                if (json[1] == '/')
                {
                    skip_oneline_comment(&json);
                }
                else if (json[1] == '*')
                {
                    skip_multiline_comment(&json);
                } else {
                    json++;
                }
                break;

            case '\"':
                minify_string(&json, (char**)&into);
                break;

            default:
                into[0] = json[0];
                json++;
                into++;
        }
    }

    /* and null-terminate. */
    *into = '\0';
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsInvalid(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_Invalid;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsFalse(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_False;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsTrue(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xff) == cJSON_True;
}


CJSON_PUBLIC(cJSON_bool) cJSON_IsBool(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & (cJSON_True | cJSON_False)) != 0;
}
CJSON_PUBLIC(cJSON_bool) cJSON_IsNull(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_NULL;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsNumber(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_Number;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsString(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_String;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsArray(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_Array;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsObject(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_Object;
}

CJSON_PUBLIC(cJSON_bool) cJSON_IsRaw(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }

    return (item->type & 0xFF) == cJSON_Raw;
}

CJSON_PUBLIC(cJSON_bool) cJSON_Compare(const cJSON * const a, const cJSON * const b, const cJSON_bool case_sensitive)
{
    if ((a == NULL) || (b == NULL) || ((a->type & 0xFF) != (b->type & 0xFF)))
    {
        return false;
    }

    /* check if type is valid */
    switch (a->type & 0xFF)
    {
        case cJSON_False:
        case cJSON_True:
        case cJSON_NULL:
        case cJSON_Number:
        case cJSON_String:
        case cJSON_Raw:
        case cJSON_Array:
        case cJSON_Object:
            break;

        default:
            return false;
    }

    /* identical objects are equal */
    if (a == b)
    {
        return true;
    }

    switch (a->type & 0xFF)
    {
        /* in these cases and equal type is enough */
        case cJSON_False:
        case cJSON_True:
        case cJSON_NULL:
            return true;

        case cJSON_Number:
            if (compare_double(a->valuedouble, b->valuedouble))
            {
                return true;
            }
            return false;

        case cJSON_String:
        case cJSON_Raw:
            if ((a->valuestring == NULL) || (b->valuestring == NULL))
            {
                return false;
            }
            if (strcmp(a->valuestring, b->valuestring) == 0)
            {
                return true;
            }

            return false;

        case cJSON_Array:
        {
            cJSON *a_element = a->child;
            cJSON *b_element = b->child;

            for (; (a_element != NULL) && (b_element != NULL);)
            {
                if (!cJSON_Compare(a_element, b_element, case_sensitive))
                {
                    return false;
                }

                a_element = a_element->next;
                b_element = b_element->next;
            }

            /* one of the arrays is longer than the other */
            if (a_element != b_element) {
                return false;
            }

            return true;
        }

        case cJSON_Object:
        {
            cJSON *a_element = NULL;
            cJSON *b_element = NULL;
            cJSON_ArrayForEach(a_element, a)
            {
                /* TODO This has O(n^2) runtime, which is horrible! */
                b_element = get_object_item(b, a_element->string, case_sensitive);
                if (b_element == NULL)
                {
                    return false;
                }

                if (!cJSON_Compare(a_element, b_element, case_sensitive))
                {
                    return false;
                }
            }

            /* doing this twice, once on a and b to prevent true comparison if a subset of b
             * TODO: Do this the proper way, this is just a fix for now */
            cJSON_ArrayForEach(b_element, b)
            {
                a_element = get_object_item(a, b_element->string, case_sensitive);
                if (a_element == NULL)
                {
                    return false;
                }

                if (!cJSON_Compare(b_element, a_element, case_sensitive))
                {
                    return false;
                }
            }

            return true;
        }

        default:
            return false;
    }
}

CJSON_PUBLIC(void *) cJSON_malloc(size_t size)
{
    return global_hooks.allocate(size);
}

CJSON_PUBLIC(void) cJSON_free(void *object)
{
    global_hooks.deallocate(object);
    object = NULL;
}
