/*
***********************************
* 本头文件定义常用的显示ANSI颜色代码
***********************************
*/

#ifndef __ELUOSI_UTIL_H__
#define __ELUOSI_UTIL_H__

#include "pub.h"

#ifdef __cplusplus
extern "C"{
#endif

/**************************************************
 * 在特定位置打印特定的字符，并伴有前景背景
 * 输入参数：usX, usY：坐标值
 *           strFgColor: 前景颜色值
 *           strBgColor: 背景颜色值
 *           cContent: 要打印的字符
 * 输出参数：无
 * 返回值  ：void
 ***************************************************/
extern void UTIL_PrintCharToPosWithoutFlush
(
    IN unsigned short usX,
    IN unsigned short usY,
    IN CONST char * strFgColor,
    IN CONST char * strBgColor,
    IN char cContent
);

/***************************************
 * 用于打印正常颜色值到终端
 * 输入参数：无
 * 输出参数：无
 * 返回值  ：void
 * *************************************
 * */
extern void UTIL_SetToNormalColor(void);

/*
函数名称:UTIL_HideCursor
函数功能:隐藏光标
输入参数:无
输出参数:无
返回值:无
*/
extern void UTIL_HideCursor(void);

/*
函数名称:UTIL_ShowCursor
函数功能:显示光标
输入参数:无
输出参数:无
返回值:无
*/
extern void UTIL_ShowCursor(void);

/*
函数功能:输出警告声音
*/
extern void UTIL_AlarmWithoutFlush();

/*
函数名称:INPUT_SetTerminalToNormal
函数功能:设置终端属性为原来的值
输入参数:无
输出参数:无
返回值: int
*/
extern int UTIL_SetTerminalToNormal(void);

/*
函数名称:INPUT_SetTerminalNoEcho
函数功能:设置终端属性为不回显
输入参数:无
输出参数:无
返回值: int
*/
int UTIL_SetTerminalNoEchoNoInputBuffer(void);

void UTIL_FlushStdout(void);

/*
函数功能:初始化终端为所需要的环境
*/
void UTIL_InitTerminal(void);


#ifdef __cplusplus
}
#endif

#endif
