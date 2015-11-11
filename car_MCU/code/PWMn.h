#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef		PWMn_H
#define		PWMn_H





#define		MAIN_Fosc	24576000L	 

#define		PCA_IDLE_DISABLE	0	 
#define		PCA_SOURCE_SELECT	4		 
										 


/**********************************************************************************************************/

#include	"STC12C5A60S2.h"

#ifdef	STC12C5201AD
	sbit  CCP0  = P3^7;	//STC12C5201AD/PWM
	sbit  CCP1  = P3^5;	//STC12C5201AD/PWM

#else
	sbit  CCP0  = P0^2;	//STC12C5A60S2
//	char  CCP0   ;	//STC12C5A60S2
	sbit  CCP1  = P1^4;	//STC12C5A60S2
#endif
#define		PWM_DUTY		 4096 			 
#define		PWM_HIGH_MAX	(PWM_DUTY-32)		 
#define		PWM_HIGH_MIN		32			 
#define     max_value   10000
sbit	P10 = P1^0;
sbit	P11 = P1^1;

sbit	P21 = P2^1;
sbit	P22 = P2^2;
sbit	P23 = P2^3;
sbit	P24= P2^4;
sbit	P44= P4^4;
sbit	P06= P0^6;
sbit	P34= P3^4;
sbit	P35= P3^5;
sbit	P33= P3^3;

#define max_rebuf  64
#define max_num   4
// receive form server data
typedef struct 
{
	int receive_len;
 	int receive_status;	/*1  : idle,0:receiving,2,wait process*/
	char   receive_buf[max_rebuf];

} Receive_D;


typedef struct 
{
	int right;
 	int left;	/* */
	int right_count;
	int left_count;

} car_status;

typedef struct 
{
	int  x;
 	int  y;	/* */
	unsigned char  x_count;
	unsigned char  y_count;
	int xn;
	int yn;

} douji_status;

void PWMn_init(unsigned int high);
void PWMn_SetHighReg(unsigned int high);
int my_atoi(const char * str,int *ret);

 void my_itoa(int n, char s[]);
#endif
