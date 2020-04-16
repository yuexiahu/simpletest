/**
 * @file simpletest.h
 * @brief 简单单元测试
 * @author hzh
 * @version 1.4
 * @date 2019-12-25
 */
#ifndef SIMPLETEST_H_
#define SIMPLETEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(WIN32)
#include <windows.h>
#define simpletest_gettick(tick)                                                                   \
    do                                                                                             \
    {                                                                                              \
        LARGE_INTEGER cur_tick;                                                                    \
        QueryPerformanceCounter(&cur_tick);                                                        \
        tick = (unsigned)cur_tick.QuadPart;                                                        \
    } while(0)
#define simpletest_output(fmt, ...)                                                                \
    do                                                                                             \
    {                                                                                              \
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);                                          \
        SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);     \
        printf(fmt, ##__VA_ARGS__);                                                                \
    } while(0)
#define simpletest_warn(fmt, ...)                                                                  \
    do                                                                                             \
    {                                                                                              \
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);                                          \
        SetConsoleTextAttribute(hStdout, FOREGROUND_RED);                                          \
        printf(fmt, ##__VA_ARGS__);                                                                \
    } while(0)
#endif

/// 打印输出函数
#ifndef simpletest_output
#define simpletest_output(fmt, ...) printf(fmt, ##__VA_ARGS__)
#endif

/// 警报输出函数
#ifndef simpletest_warn
#define simpletest_warn(fmt, ...) simpletest_output("\e[31m" fmt "\e[0m", ##__VA_ARGS__)
#endif

/// 在包含头文件前定义，可打印输出通过的测试
#ifdef SIMPLETEST_ENABLE_DEBUG
#define simpletest_debug(fmt, ...) simpletest_output(fmt, ##__VA_ARGS__)
#else
#define simpletest_debug(fmt, ...)
#endif

/// 微妙级时间获取函数
#ifndef simpletest_gettick
#define simpletest_gettick(tick)                                                                   \
    do                                                                                             \
    {                                                                                              \
        tick = clock() * (1000000 / CLOCKS_PER_SEC);                                               \
    } while(0)
#endif

/// 运行测试用例
#define RUN_TEST(test_case)                                                                        \
    do                                                                                             \
    {                                                                                              \
        unsigned start_tick_, end_tick_;                                                           \
        double pass_ = 100;                                                                        \
        simpletest_output("==========================================================\n");         \
        simpletest_output("RUN_TEST: " #test_case "\n");                                           \
        simpletest_reset();                                                                        \
        simpletest_gettick(start_tick_);                                                           \
        test_case();                                                                               \
        simpletest_gettick(end_tick_);                                                             \
        if(simpletest_count() > 1)                                                                 \
        {                                                                                          \
            pass_ = simpletest_pass() * 100.0 / simpletest_count();                                \
        }                                                                                          \
        if(simpletest_pass() < simpletest_count())                                                 \
        {                                                                                          \
            simpletest_warn("FAILED  : %d/%d (%3.2f%%) in %0.3f ms\n", simpletest_pass(),          \
                            simpletest_count(), pass_, (end_tick_ - start_tick_) / 1000.);         \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            simpletest_output("PASSED  : %d/%d (%3.2f%%) in %0.3f ms\n", simpletest_pass(),        \
                              simpletest_count(), pass_, (end_tick_ - start_tick_) / 1000.);       \
        }                                                                                          \
    } while(0)

/**
 * @brief 期望表达式为真，输出指定内容
 * @param require 条件是否必须成功,非0将在测试失败后结束程序
 * @param expression 表达式
 * @param format 格式化输出字符串
 * @param ... 输出变参
 */
#define TEST(require, expression, format, ...)                                                     \
    do                                                                                             \
    {                                                                                              \
        int result = simpletest_test(expression);                                                  \
        if(!result)                                                                                \
        {                                                                                          \
            simpletest_warn("  [%d] %s:%s:%d: FAILED:\n" format "\n", simpletest_count(),          \
                            simpletest_truncat_path(__FILE__), __FUNCTION__, __LINE__,             \
                            ##__VA_ARGS__);                                                        \
            if(require)                                                                            \
            {                                                                                      \
                exit(1);                                                                           \
            }                                                                                      \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            simpletest_debug("  [%d] %s:%s:%d: PASSED:\n" format "\n", simpletest_count(),         \
                             simpletest_truncat_path(__FILE__), __FUNCTION__, __LINE__,            \
                             ##__VA_ARGS__);                                                       \
        }                                                                                          \
    } while(0)

#define STRINGFY_FUNC(func, ...) #func "(" #__VA_ARGS__ ")"

#define TEST_CMP(title, require, expect, cmp, actual, type, format)                                \
    do                                                                                             \
    {                                                                                              \
        type expect_ = (expect);                                                                   \
        type actual_ = (actual);                                                                   \
        TEST((require), expect_ cmp actual_,                                                       \
             "  %s\n"                                                                              \
             "    ==>  %s " #cmp " %s\n"                                                           \
             "    ==>  " format " " #cmp " " format "\n"                                           \
             "    ==>  %s\n",                                                                      \
             title, #expect, #actual, expect_, actual_, (result ? "true" : "false"));              \
    } while(0)

#define TEST_OP(title, require, op, expect, actual, type, format, ...)                             \
    do                                                                                             \
    {                                                                                              \
        type expect_ = (expect);                                                                   \
        type actual_ = (actual);                                                                   \
        TEST((require), op(expect_, actual_, ##__VA_ARGS__),                                       \
             "  %s\n"                                                                              \
             "    ==>  " STRINGFY_FUNC(op, expect, actual, ##__VA_ARGS__)"\n"                      \
             "    ==>  " STRINGFY_FUNC(op, format, format, ##__VA_ARGS__)"\n"                      \
             "    ==>  %s\n",                                                                      \
             title, expect_, actual_, (result ? "true" : "false"));                                \
    } while(0)

#define TEST_EXP(title, require, expression, format, ...)                                          \
    TEST((require), (expression),                                                                  \
         "  %s\n"                                                                                  \
         "    ==>  %s\n"                                                                           \
         "    ==>  %s\n  " format,                                                                 \
         title, #expression, (result ? "true" : "false"), ##__VA_ARGS__)

/**
 * @brief 期望表达式结果为真
 * @param expression 表达式
 * @param ... 额外输出信息
 */
#define EXPECT(expression, ...)                                                                    \
    TEST_EXP(STRINGFY_FUNC(EXPECT, expression, ##__VA_ARGS__), 0, expression, "" __VA_ARGS__)

/**
 * @brief 要求表达式结果为真
 * @param expression 表达式
 * @param ... 额外输出信息
 */
#define REQUIRE(expression, ...)                                                                   \
    TEST_EXP(STRINGFY_FUNC(REQUIRE, expression, ##__VA_ARGS__), 1, expression, "" __VA_ARGS__)

/**
 * @brief 期望表达式结果为假
 * @param expression 表达式
 * @param ... 额外输出信息
 */
#define EXPECT_FALSE(expression, ...)                                                              \
    TEST_EXP(STRINGFY_FUNC(EXPECT_FALSE, expression, ##__VA_ARGS__), 0, !(expression),             \
             "" __VA_ARGS__)
/**
 * @brief 要求表达式结果为假
 * @param expression 表达式
 * @param ... 额外输出信息
 */
#define REQUIRE_FALSE(expression, ...)                                                             \
    TEST_EXP(STRINGFY_FUNC(REQUIRE_FALSE, expression, ##__VA_ARGS__), 1, !(expression),            \
             "" __VA_ARGS__)

/**
 * @brief 期望2个整数表达式相等
 * @param expect 期望表达式
 * @param actual 实际表达式
 */
#define EXPECT_EQ_INT(expect, actual)                                                              \
    TEST_CMP(STRINGFY_FUNC(EXPECT_EQ_INT, expect, actual), 0, expect, ==, actual, int, "%d")
/**
 * @brief 要求2个整数表达式相等
 * @param expect 期望表达式
 * @param actual 实际表达式
 */
#define REQUIRE_EQ_INT(expect, actual)                                                             \
    TEST_CMP(STRINGFY_FUNC(REQUIRE_EQ_INT, expect, actual), 1, expect, ==, actual, int, "%d")

/**
 * @brief 期望2个浮点数表达式相等
 * @param expect 期望表达式
 * @param actual 实际表达式
 */
#define EXPECT_EQ_DOUBLE(expect, actual)                                                           \
    TEST_CMP(STRINGFY_FUNC(EXPECT_EQ_DOUBLE, expect, actual), 0, expect, ==, actual, double, "%f")
/**
 * @brief 要求2个浮点数表达式相等
 * @param expect 期望表达式
 * @param actual 实际表达式
 */
#define REQUIRE_EQ_DOUBLE(expect, actual)                                                          \
    TEST_CMP(STRINGFY_FUNC(REQUIRE_EQ_DOUBLE, expect, actual), 1, expect, ==, actual, double, "%f")

/**
 * @brief 期望2个字符串表达式相等
 * @param expect 期望表达式
 * @param actual 实际表达式
 */
#define EXPECT_EQ_STR(expect, actual)                                                              \
    TEST_OP(STRINGFY_FUNC(EXPECT_EQ_STR, expect, actual), 0, simpletest_eq_str, expect, actual,    \
            const char*, "%s")
/**
 * @brief 期望2个字符串表达式前len个字符相等
 * @param expect 期望表达式
 * @param actual 实际表达式
 * @param len 字符长度
 */
#define EXPECT_EQ_STRN(expect, actual, len)                                                        \
    TEST_OP(STRINGFY_FUNC(EXPECT_EQ_STRN, expect, actual), 0, simpletest_eq_strn, expect, actual,  \
             const char*, "%s", len)
/**
 * @brief 要求2个字符串表达式相等
 * @param expect 期望表达式
 * @param actual 实际表达式
 */
#define REQUIRE_EQ_STR(expect, actual)                                                             \
    TEST_OP(STRINGFY_FUNC(REQUIRE_EQ_STR, expect, actual), 1, simpletest_eq_str, expect, actual,   \
            const char*, "%s")
/**
 * @brief 要求2个字符串表达式前len个字符相等
 * @param expect 期望表达式
 * @param actual 实际表达式
 * @param len 字符长度
 */
#define REQUIRE_EQ_STRN(expect, actual, len)                                                       \
    TEST_OP(STRINGFY_FUNC(REQUIRE_EQ_STRN, expect, actual), 1, simpletest_eq_strn, expect,         \
             actual, const char*, "%s", len)

/**
 * @brief 期望2块内存相等
 * @param expect 期望表达式
 * @param actual 实际表达式
 * @param len 内存长度
 */
#define EXPECT_EQ_MEM(expect, actual, len)                                                         \
    TEST_OP(STRINGFY_FUNC(EXPECT_EQ_MEM, expect, actual), 0, simpletest_eq_mem, expect, actual,    \
             const void*, %p, len)
/**
 * @brief 要求2块内存相等
 * @param expect 期望表达式
 * @param actual 实际表达式
 * @param len 内存长度
 */
#define REQUIRE_EQ_MEM(expect, actual, len)                                                        \
    TEST_OP(STRINGFY_FUNC(REQUIRE_EQ_MEM, expect, actual), 1, simpletest_eq_mem, expect, actual,   \
             const void*, %p, len)

/**
 * @brief 执行一次测试，记录结果
 *
 * @param equality 测试条件，0失败，非0成功
 * @param tick 测试执行时间
 *
 * @return 本次测试结果
 *   @retval 0 失败
 *   @retval 1 成功
 */
int simpletest_test(int equality);

/**
 * @brief 清空测试记录
 */
void simpletest_reset();

/**
 * @brief 返回总体测试结果
 *
 * @return 总体测试结果
 *   @retval 0 失败
 *   @retval 1 成功
 */
int simpletest_result();

/**
 * @brief 获取测试次数
 *
 * @return 测试次数
 */
int simpletest_count();

/**
 * @brief 获取通过次数
 *
 * @return 测试通过次数
 */
int simpletest_pass();

/**
 * @brief 字符串比较函数，支持NULL
 *
 * @param s1 字符串1
 * @param s2 字符串2
 *
 * @return 是否相同
 *   @retval 0 不同
 *   @retval 1 相同
 */
int simpletest_eq_str(const char* s1, const char* s2);

/**
 * @brief 字符串比较函数，支持NULL
 * 最多比较前len个字符长度，遇到'\0'会停止
 * @param s1 字符串1
 * @param s2 字符串2
 * @param len 字符长度
 *
 * @return 是否相同
 *   @retval 0 不同
 *   @retval 1 相同
 */
int simpletest_eq_strn(const char* s1, const char* s2, size_t len);

/**
 * @brief 内存比较函数，支持NULL
 *
 * @param m1 内存地址1
 * @param m2 内存地址2
 * @param len 内存长度
 *
 * @return 是否相同
 *   @retval 0 不同
 *   @retval 1 相同
 */
int simpletest_eq_mem(const void* m1, const void* m2, size_t len);

/**
 * @brief 从路径中截取文件名
 *
 * @param path 路径字符串
 *
 * @return 文件名
 */
const char* simpletest_truncat_path(const char* path);

/**
 * @brief 必须在主函数外定义一次，包含相关函数定义
 */
#define SIMPLETEST_DEF()                                                                           \
    static int test_result_ = 1;                                                                   \
    static int test_count_ = 0;                                                                    \
    static int test_pass_ = 0;                                                                     \
    int simpletest_test(int equality)                                                              \
    {                                                                                              \
        ++test_count_;                                                                             \
        if(equality)                                                                               \
        {                                                                                          \
            ++test_pass_;                                                                          \
            return 1;                                                                              \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            test_result_ = 0;                                                                      \
        }                                                                                          \
        return 0;                                                                                  \
    }                                                                                              \
    void simpletest_reset()                                                                        \
    {                                                                                              \
        test_count_ = 0;                                                                           \
        test_pass_ = 0;                                                                            \
    }                                                                                              \
    int simpletest_result()                                                                        \
    {                                                                                              \
        return test_result_;                                                                       \
    }                                                                                              \
    int simpletest_count()                                                                         \
    {                                                                                              \
        return test_count_;                                                                        \
    }                                                                                              \
    int simpletest_pass()                                                                          \
    {                                                                                              \
        return test_pass_;                                                                         \
    }                                                                                              \
    int simpletest_eq_str(const char* s1, const char* s2)                                          \
    {                                                                                              \
        if(s1 == s2)                                                                               \
        {                                                                                          \
            return 1;                                                                              \
        }                                                                                          \
        if(s1 == NULL || s2 == NULL)                                                               \
        {                                                                                          \
            return 0;                                                                              \
        }                                                                                          \
        return strcmp(s1, s2) == 0;                                                                \
    }                                                                                              \
    int simpletest_eq_strn(const char* s1, const char* s2, size_t n)                               \
    {                                                                                              \
        if(s1 == s2 || n == 0)                                                                     \
        {                                                                                          \
            return 1;                                                                              \
        }                                                                                          \
        if(s1 == NULL || s2 == NULL)                                                               \
        {                                                                                          \
            return 0;                                                                              \
        }                                                                                          \
        return strncmp(s1, s2, n) == 0;                                                            \
    }                                                                                              \
    int simpletest_eq_mem(const void* m1, const void* m2, size_t n)                                \
    {                                                                                              \
        if(m1 == m2 || n == 0)                                                                     \
        {                                                                                          \
            return 1;                                                                              \
        }                                                                                          \
        if(m1 == NULL || m2 == NULL)                                                               \
        {                                                                                          \
            return 0;                                                                              \
        }                                                                                          \
        return memcmp(m1, m2, n) == 0;                                                             \
    }                                                                                              \
    const char* simpletest_truncat_path(const char* path)                                          \
    {                                                                                              \
        const char* p = path;                                                                      \
        if(path == NULL)                                                                           \
        {                                                                                          \
            return NULL;                                                                           \
        }                                                                                          \
        while(*p)                                                                                  \
        {                                                                                          \
            if(*p == '/')                                                                          \
            {                                                                                      \
                path = p + 1;                                                                      \
            }                                                                                      \
            ++p;                                                                                   \
        }                                                                                          \
        return path;                                                                               \
    }                                                                                              \
    /*SIMPLETEST_DEF() END*/

#endif // SIMPLETEST_H_
