/*
***********************************
* 本头文件定义常用的显示ANSI颜色代码
***********************************
*/
#ifndef __ELUOSI_COLOR_H__
#define __ELUOSI_COLOR_H__

#ifdef __cplusplus
extern "C"{
#endif

#define COLOR_FG_RED        "\033[31m"
#define COLOR_FG_GREEN      "\033[32m"
#define COLOR_FG_YELLOW     "\033[33m"
#define COLOR_BG_RED        "\033[41m"
#define COLOR_BG_GREEN      "\033[42m"
#define COLOR_BG_YELLOW     "\033[43m"

#define COLOR_NORMAL "\033[00m"

#ifdef __cplusplus
}
#endif

#endif
