
#include <signal.h>
#include <stdlib.h>


#include "util.h"
#include "colors.h"
#include "pub.h"
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>


#define LED_RED   0
#define LED_GREEN   1
#define LED_NONE 	3 //代表LED灯不亮

#define LED_ON   0
#define LED_OFF   1

int g_led_pos[4][2] = 
{
	{23, 37},
	{26, 37},
	{29, 37},
	{32, 37},
};


void set_led(int ledno, int color, int onoff)
{
	if (LED_OFF == onoff)
	{
		UTIL_PrintCharToPosWithoutFlush(
		g_led_pos[ledno][0], g_led_pos[ledno][1],
		COLOR_NORMAL, COLOR_NORMAL, ' '
		);
		UTIL_FlushStdout();
		return ;
	}

	if (LED_RED == color)
	{
		UTIL_PrintCharToPosWithoutFlush(
		g_led_pos[ledno][0], g_led_pos[ledno][1],
		COLOR_FG_RED, COLOR_BG_RED, ' '
		);
	}
	else if (LED_GREEN == color)
	{
		UTIL_PrintCharToPosWithoutFlush(
		g_led_pos[ledno][0], g_led_pos[ledno][1],
		COLOR_FG_GREEN, COLOR_BG_GREEN, ' '
		);
	}
	UTIL_FlushStdout();
}

void ctrl_handler(int signo)  /*ctrl + c*/
{
	UTIL_PrintCharToPosWithoutFlush(
	1, 1,
	COLOR_NORMAL, COLOR_NORMAL, ' '
	);

	UTIL_SetTerminalToNormal();
	exit(0);
}

typedef struct stLED_STAT
{
	int led_conf_color;
	int led_conf_ontime;
	int led_conf_offtime;
	
	int led_cur_state;
	int led_cur_timeused;
	int led_cur_lefttime;
} LED_STATE;

#define MAX_LED_NUM 4

LED_STATE g_stLedStat[MAX_LED_NUM];
#define NOT_CHANGE 1
#define CHANGED   0

int g_isChange = NOT_CHANGE;

enum enFreq
{
	FREQ_1HZ,
	FREQ_0_5HZ,
	FREQ_2HZ,
	FREQ_4HZ,
	FREQ_8HZ,
	FREQ_0_25HZ
};

struct conf
{
	char ledno;
	char space1;
	char ischange;
	char space2;
	char color;
	char space3;
	char freq;
	char seq;
};

extern void calc_onoff(enum enFreq freq, int *on, int *off);
struct LED_CONF
{
	enum enFreq freq;
	int led_color;
	int led_ischange;
};
struct LED_CONF ledconf[4] = {0};
void parsing_conf(void)
{
	 int file = open( "./led_conf", O_RDONLY);
	 if (-1 == file)
	 {
		printf("open file failed\n");
	 }
	 unsigned char buf[1024] = {0};

	 int file_len = read(file, buf, sizeof(buf));


	 enum enFreq freq[4];
	 struct conf *p = (struct conf *)buf;
	int i;
	for(i = 0; i < 4; ++i)
	{
		if ('1' == p[i].ischange)
			ledconf[i].led_ischange = CHANGED;
		else
			ledconf[i].led_ischange = NOT_CHANGE;
		if ('0' == p[i].color)
			ledconf[i].led_color = LED_GREEN;
		else
			ledconf[i].led_color = LED_RED;
		switch (p[i].freq)
		{
			case '0':
				ledconf[i].freq = FREQ_0_5HZ;
				break ;
			case '1':
				ledconf[i].freq = FREQ_1HZ;
				break ;
			case '2':
				ledconf[i].freq = FREQ_2HZ;
				break ;
			case '4':
				ledconf[i].freq = FREQ_4HZ;
				break ;
			case '5':
				ledconf[i].freq = FREQ_0_25HZ;
				break ;
			case '8':
				ledconf[i].freq = FREQ_8HZ;
				break ;
		}
	}
	close(file);
}

void parsing_conf2(void)
{
	int i;
	int on;
	int off; int chgflag = 0;
	for (i = 0; i < 4; i++)
	{
		calc_onoff(ledconf[i].freq, &on, &off);
		g_stLedStat[i].led_conf_ontime = on;
		g_stLedStat[i].led_conf_offtime = off;
		g_stLedStat[i].led_conf_color = ledconf[i].led_color;

		if (CHANGED == ledconf[i].led_ischange)
		{
			chgflag = 1;
		}
	}
	
	if (1 == chgflag)
	{
		g_isChange = CHANGED;
	}
}

void sigstp_handle(int no) /*ctrl + z*/
{
	parsing_conf();
	parsing_conf2();
}

int main()
{
	struct sigaction crtlc;
	crtlc.sa_handler = ctrl_handler;
	sigemptyset(&crtlc.sa_mask);
	crtlc.sa_flags = 0;
	sigaction(SIGINT, &crtlc, NULL);

	struct sigaction sigstp;
	sigstp.sa_handler = sigstp_handle;
	sigemptyset(&sigstp.sa_mask);
	sigstp.sa_flags = 0;
	sigaction(SIGTSTP, &sigstp, NULL);
	
	int i;
	UTIL_InitTerminal();
	/*
	parsing config para		
	*/
	g_stLedStat[0].led_conf_color = LED_GREEN;
	g_stLedStat[0].led_conf_ontime = 1000000; /*1s*/
	g_stLedStat[0].led_conf_offtime = 1000000; /*1s*/
	g_stLedStat[0].led_cur_state = LED_OFF;
	g_stLedStat[0].led_cur_timeused	= 0;
	g_stLedStat[0].led_cur_lefttime = 1000000;  /*剩余1s*/

	g_stLedStat[1].led_conf_color = LED_GREEN;
	g_stLedStat[1].led_conf_ontime = 250000; /*0.25s*/
	g_stLedStat[1].led_conf_offtime = 250000; /*0.25s*/
	g_stLedStat[1].led_cur_state = LED_OFF;
	g_stLedStat[1].led_cur_timeused	= 0;
	g_stLedStat[1].led_cur_lefttime = 1000000;  /*剩余1s*/

	g_stLedStat[2].led_conf_color = LED_GREEN;
	g_stLedStat[2].led_conf_ontime = 1000000; /*1s*/
	g_stLedStat[2].led_conf_offtime = 1000000; /*1s*/
	g_stLedStat[2].led_cur_state = LED_OFF;
	g_stLedStat[2].led_cur_timeused	= 0;
	g_stLedStat[2].led_cur_lefttime = 1000000;  /*剩余1s*/

	g_stLedStat[3].led_conf_color = LED_RED;
	g_stLedStat[3].led_conf_ontime = 100000; /*1s*/
	g_stLedStat[3].led_conf_offtime = 100000; /*1s*/
	g_stLedStat[3].led_cur_state = LED_OFF;
	g_stLedStat[3].led_cur_timeused	= 0;
	g_stLedStat[3].led_cur_lefttime = 1000000;  /*剩余1s*/
	

	for(;;)
	{
		/*解析配置，同步配置*/
		/****下面进行同步，做到同步点亮，同步点灭***/
		/*sigstp_handle 
		处理的时候，并没有同步灯的亮灭，这里需要把改变的灯的状态与未改变的同步即可*/
		//1,找到没有改变状态的灯
		int unchgled = -1; 
		int maxlefttime = -1;
		for (i = 0; i < 4; ++i)
		{
			if (NOT_CHANGE == ledconf[i].led_ischange)
			{
				if (g_stLedStat[i].led_cur_lefttime > maxlefttime)
				{
					unchgled = i;
					maxlefttime = g_stLedStat[i].led_cur_lefttime;
				}
			}
		}

		/*2,开始同步数据*/
		if (-1 == unchgled) /*没找到的情况下*/
		{
			for (i = 0; i < 4; i++)
			{
				g_stLedStat[i].led_cur_lefttime = g_stLedStat[i].led_conf_offtime;
				g_stLedStat[i].led_cur_state = LED_OFF;
			}
		}
		else
		{
			for (i = 0; i < 4; i++)
			{
				if (CHANGED == ledconf[i].led_ischange)
				{
					g_stLedStat[i].led_cur_state = g_stLedStat[unchgled].led_cur_state;
					g_stLedStat[i].led_cur_lefttime = (LED_OFF == g_stLedStat[i].led_cur_state ?
							g_stLedStat[i].led_conf_offtime : g_stLedStat[i].led_conf_ontime);
				}
			}
		}
		


		/*开始执行一种状态*/
		g_isChange = NOT_CHANGE;
		while(NOT_CHANGE == g_isChange)
		{
			int ministtime = g_stLedStat[0].led_cur_lefttime;
			/*显示当前状态, 并找到最小的时间*/
			for (i = 0; i < 4; i++)
			{
				set_led(i, g_stLedStat[i].led_conf_color, g_stLedStat[i].led_cur_state);
				if (g_stLedStat[i].led_cur_lefttime < ministtime)
				{
					ministtime = g_stLedStat[i].led_cur_lefttime;/*找到需要最小睡眠的时间*/
				}
			}
			/*睡眠最小的时间*/
			usleep(ministtime);
			/*更新led状态数据*/
			for (i = 0; i < 4; i++)
			{
				g_stLedStat[i].led_cur_lefttime -= ministtime;
				if (0 == g_stLedStat[i].led_cur_lefttime)
				{
					/*剩余时间为0，则反转LED的状态*/
					g_stLedStat[i].led_cur_state = (LED_OFF == g_stLedStat[i].led_cur_state ? LED_ON : LED_OFF);
					g_stLedStat[i].led_cur_lefttime = (LED_OFF == g_stLedStat[i].led_cur_state ? 
									g_stLedStat[i].led_conf_offtime : g_stLedStat[i].led_conf_ontime);
				}
			}
			
		}
		
	}

	UTIL_SetTerminalToNormal();
}

void calc_onoff(enum enFreq freq, int *on, int *off)
{	
	switch (freq)
	{
		case FREQ_1HZ:
			*on = 500000;
			*off = 500000;
			break;
		case FREQ_0_5HZ:
			*on = 1000000;
			*off = 1000000;
			break;
		case FREQ_2HZ:
			*on = 1000000/4;
			*off = 1000000/4;
			break;
		case FREQ_4HZ:
			*on = 1000000/8;
			*off = 1000000/8;
			break;
		case FREQ_8HZ:
			*on = 1000000/16;
			*off = 1000000/16;
			break;
		case FREQ_0_25HZ:
			*on = 1000000;
			*off = 3000000;
			break;
		default:
			*on = 1000000;
			*off = 1000000;
			break;
	}
	return ;
}
