/***************************************************
 * 本模块提供在标准输出的特定位置打印ansi字符、
 * 创建线程、进程的封装函数等。
 *
 *************************************************/
#ifdef __cplusplus
extern "C"{
#endif

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include "pub.h"
#include "colors.h"
#include <termios.h>
#include "util.h"


static struct termios g_stOldTerminalAttr;
static bool g_bOldTerminalAttrAvailabe = false;
//static struct termios g_stNewTerminalAttr; 


/*
函数名称:UTIL_HideCursor
函数功能:隐藏光标
输入参数:无
输出参数:无
返回值:无
*/
void UTIL_HideCursor(void)
{
    printf("\033[?25l");
    fflush(stdout);

    return ;
}

/*
函数名称:UTIL_ShowCursor
函数功能:显示光标
输入参数:无
输出参数:无
返回值:无
*/
void UTIL_ShowCursor(void)
{
    printf("\033[?25h");
    fflush(stdout);

    return ;
}

/**************************************************
 * 在特定位置打印特定的字符，并伴有前景背景
 * 输入参数：usX, usY：坐标值
 *           strFgColor: 前景颜色值
 *           strBgColor: 背景颜色值
 *           cContent: 要打印的字符
 * 输出参数：无
 * 返回值  ：void
 ***************************************************/
void UTIL_PrintCharToPosWithoutFlush
(
    IN unsigned short usX,
    IN unsigned short usY,
    IN CONST char * strFgColor,
    IN CONST char * strBgColor,
    IN char cContent
)
{
    printf("\033[%hi;%hiH%s%s%c", usY, usX, strFgColor, strBgColor, cContent);
    return ;
}
/***************************************
 * 用于打印正常颜色值到终端
 * 输入参数：无
 * 输出参数：无
 * 返回值  ：void
 * *************************************
 * */
void UTIL_SetToNormalColor(void)
{
    printf("%s", COLOR_NORMAL);
}


/*
函数功能:输出警告声音
*/
void UTIL_AlarmWithoutFlush()
{
    printf("\a");
}

/*
函数名称:INPUT_SetTerminalToNormal
函数功能:设置终端属性为原来的值
输入参数:无
输出参数:无
返回值: int
*/
int UTIL_SetTerminalToNormal(void)
{
    if (true != g_bOldTerminalAttrAvailabe)
    {
        return ERROR_FAILED;
    }

    
    /*显示光标*/
    UTIL_ShowCursor();

    /*恢复正常颜色显示*/
    UTIL_SetToNormalColor();

    /*恢复原来终端属性, 添加输入缓冲,回显*/
    if (0 != tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_stOldTerminalAttr))
    {
        perror("terminal recover failed: ");
        return ERROR_FAILED;
    }
    
    return ERROR_SUCCESS;
}

/*
函数功能:初始化终端为所需要的环境
*/
void UTIL_InitTerminal(void)
{
    /*获取终端属性,并保存下来.*/
    if (0 != tcgetattr(STDIN_FILENO, &g_stOldTerminalAttr) )
    {
        perror("tcgetattr fuction:");
    }
    g_bOldTerminalAttrAvailabe = true;

    /*设置终端*/
    UTIL_SetTerminalNoEchoNoInputBuffer();
}

/*
函数名称:INPUT_SetTerminalNoEcho
函数功能:设置终端属性为不回显,且没有输入缓冲
输入参数:无
输出参数:无
返回值: int
*/
int UTIL_SetTerminalNoEchoNoInputBuffer(void)
{
    struct termios stTerminalAttr;

    /*获取当前终端属性*/
    if (0 != tcgetattr(STDIN_FILENO, &stTerminalAttr) )
    {
        perror("tcgetattr function: ");
        return ERROR_FAILED;
    }

    /*隐藏光标*/
    UTIL_HideCursor();

    /*设置终端输入不回显*/
    stTerminalAttr.c_lflag &= ~ECHO;
    stTerminalAttr.c_lflag &= ~ICANON;

    /*设置最小等待字符*/
    stTerminalAttr.c_cc[VMIN] = 1;
    /*设置最小等待时间*/
    stTerminalAttr.c_cc[VTIME] = 0;

    if (0 != tcsetattr(STDIN_FILENO, TCSAFLUSH, &stTerminalAttr))
    {
        perror("tcsetattr function: ");
        return ERROR_FAILED;
    }

    return ERROR_SUCCESS;
}

void UTIL_FlushStdout(void)
{
    fflush(stdout);
}

#ifdef __cplusplus
}
#endif
