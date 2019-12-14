/*
***********************************
* 此头文件定义本游戏常用的一些宏
***********************************
*/
#ifndef __ELUOSI_PUB_H__
#define __ELUOSI_PUB_H__

#ifdef __cplusplus
extern "C"{
#endif

#define IN 
#define OUT  
#define INOUT  

#define CONST const
#define VOID void
#define ERROR_SUCCESS 0X00
#define ERROR_FAILED  0X01

#define THIS_MODUAL_IS_INIT true

/*是否打开断言开关*/
//#define NDEBUG  /*关闭断言调试功能*/
#include <assert.h>

typedef unsigned short USHORT;
typedef short SHORT;

#ifdef __cplusplus
}
#endif

#endif
