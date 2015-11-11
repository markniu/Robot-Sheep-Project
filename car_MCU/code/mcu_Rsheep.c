
/*

luojin012@126.com
/*------------------------------------------------------------------*/

#include	"PWMn.h"

#define MAIN_Fosc		11059200L	//define main clock
#define Baudrate1		9600		//define the baudrate,  
#define Baudrate2		9600		//define the baudrate2,
#define		BUF_LENTH	64		 
#define T1_TimerReload	(256 - MAIN_Fosc / 192 / Baudrate1)			//Calculate the timer1 reload value	at 12T mode
#define BRT_Reload		(256 - MAIN_Fosc / 12 / 16 / Baudrate2)		//Calculate BRT reload value

#define	TimeOut1		(28800 / (unsigned long)Baudrate1 + 2)
#define	TimeOut2		(28800 / (unsigned long)Baudrate2 + 2)

#define	TI2				(S2CON & 0x02) != 0
#define	RI2				(S2CON & 0x01) != 0
#define	CLR_TI2()		S2CON &= ~0x02
#define	CLR_RI2()		S2CON &= ~0x01


unsigned char 	uart1_wr;		 
unsigned char 	uart1_rd;		 
bit		B_TI;

unsigned char 	uart2_wr;		 
unsigned char 	uart2_rd;		 
unsigned char 	xdata RX2_Buffer[BUF_LENTH];
bit		B_TI2;

sbit pwmd =P0^6 ;          //PWM output pin
sbit pwmd1 =P4^4 ;          //PWM output pin
unsigned int	pwm;
unsigned int count;      //0.5ms 
unsigned char jd;         // 
Receive_D  Receive_CMD[max_num];
car_status   car;
douji_status  douji;
int idle_buf=0;

void	uart1_init(void);
void	uart2_init(void);
void	UART1_TxByte(unsigned char dat);
void	UART2_TxByte(unsigned char dat);
void	PrintString1(unsigned char code *puts);
void	PrintString2(unsigned char code *puts);




void receing_data(char datar)
{

	if(datar==';')
	{
		Receive_CMD[idle_buf].receive_buf[Receive_CMD[idle_buf].receive_len++] = 0;	
		Receive_CMD[idle_buf].receive_status=2;
		idle_buf++;
		/////
		if(idle_buf>=max_num)
			idle_buf=0;
		
	}
	else  if(Receive_CMD[idle_buf].receive_status<2)
	{
		Receive_CMD[idle_buf].receive_buf[Receive_CMD[idle_buf].receive_len++] = datar;	
		Receive_CMD[idle_buf].receive_status=1;
		if(Receive_CMD[idle_buf].receive_len>max_rebuf) 
			Receive_CMD[idle_buf].receive_len=0;

	}


}

void UART2_RCV (void) interrupt 8
{
	char datar;
	if(RI2)
	{
		 
		CLR_RI2();
		datar=S2BUF;
		receing_data(datar);

	}

	if(TI2)
	{
		CLR_TI2();
		B_TI2 = 1;
	}

}
void	UART1_TxByte(unsigned char dat)
{
	B_TI = 0;
	SBUF = dat;
	while(!B_TI);
	B_TI = 0;
}

void	UART2_TxByte(unsigned char dat)
{
	B_TI2 = 0;
	S2BUF = dat;
	while(!B_TI2);
	B_TI2 = 0;
}

void PrintString1(unsigned char   *puts)		 
{
    for (; *puts != 0;	puts++)  UART1_TxByte(*puts); 	 
}

void PrintString2(unsigned char   *puts)		 
{
    for (; *puts != 0;	puts++)  UART2_TxByte(*puts); 	 
}


void Time0_Init()           
{
	TMOD = 0x01;          
	IE   = 0x82;
	TH0  = 0xfe;
	TL0  = 0x33;     //11.0592MZ £¬0.5ms
 	TR0=1;                 
 }




void cmd_pase(char *data_c,int len)
{
 	int i;
	int j=0,tmp=0;
	//char *data_c=Receive_CMD[0].receive_buf;
	memset(RX2_Buffer,0,BUF_LENTH);
			
	for(i=0;i<len;i++)
	{
		if(data_c[i]=='r')		   // right motor speed
		{
			
			car.right=atoi(data_c+i+1);
			sprintf(RX2_Buffer,"car.right=%d\n\0",car.right);	
			PrintString1(RX2_Buffer);
			break;
		}
		else if(data_c[i]=='l')	   // left motor speed
		{
		 	car.left=  atoi(data_c+i+1);
			sprintf(RX2_Buffer,"car.left=%d\n\0",car.left);	
			PrintString1(RX2_Buffer);
			break;
		}
		else if(data_c[i]=='b')	   // both motor speed
		{
		 	car.right=car.left=  atoi(data_c+i+1);
			sprintf(RX2_Buffer,"car.right=car.left=%d\n\0",car.left);	
			PrintString1(RX2_Buffer);
			break;
		}
		else if(data_c[i]=='x')	   // both motor speed
		{
		 	douji.x=  atoi(data_c+i+1);
			douji.x_count=0;
			douji.xn=0;
			sprintf(RX2_Buffer,"douji.x=%d\n\0",douji.x);	
			PrintString1(RX2_Buffer);
			break;
		}
		else if(data_c[i]=='y')	   // both motor speed
		{
		 	douji.y=  atoi(data_c+i+1);
			douji.y_count=0;
			douji.yn=0;
			sprintf(RX2_Buffer,"douji.y=%d\n\0",douji.y);	
			PrintString1(RX2_Buffer);
			break;
		}

			
	}
}


#define OSC_FREQ (11059200UL)
#define UINT16  unsigned short

#define OSC_PER_INST (12)


#define PRELOAD500uS (65536 - 2*(UINT16)(OSC_FREQ/(OSC_PER_INST*1000)))
#define PRELOAD1MS (65536 - 4*(UINT16)(OSC_FREQ/(OSC_PER_INST*1000)))
#define PRELOAD5MS (65536 - 5*(UINT16)(OSC_FREQ/(OSC_PER_INST*1000)))

void start_Timeout1ms(){
	TH0 = PRELOAD1MS / 256;
	TL0 = PRELOAD1MS % 256;
	TF0 = 0 ;//clear overflow bit

	ET0 = 1;    //close timer IRQ

	TR0 = 1;    
	//TMOD = 0x01;           
	//IE   = 0x82;

}
void start_Timeout500uS(){
	TH0 = PRELOAD500uS / 256;
	TL0 = PRELOAD500uS % 256;
	TF0 = 0 ;//clear overflow bit

	ET0 = 1;    //close timer IRQ
 
 	TR0 = 1;     
	//TMOD = 0x01;            
	//IE   = 0x82;

}

void start_Timeout5ms(){
    //TMOD = ;

    TH0 = PRELOAD5MS / 256;
    TL0 = PRELOAD5MS % 256;
    TF0 = 0 ;// 

    ET0 = 0;    // 

    TR0 = 1;    // 

}

void stop_Timeout(){
    TR0 = 0;
}


void car_control()
{
 	if(car.left||car.right)
 		P34=1;
 	else
 	{
 		return;
	 	
 	}
	car.left_count++;
	if(car.left_count==10)
	{
		car.left_count=0;
		P11=0;		
	 	P33=0;
	}

	if(car.left==0)
	{ // stop
	 	P11=0;		
	 	P33=0;	
	}	 
	if(car.left==car.left_count)
 		P11=1;		
	
	if(car.left==(-car.left_count))
 		P33=1;		

////////////
	car.right_count++;
	if(car.right_count==10)
	{
		car.right_count=0;
		P21=0;		
	 	P10=0;
	}

	if(car.right==0)
	{ // stop
	 	P21=1;		
	 	P10=1;	
	}	 
	if(car.right==car.right_count)
 		P21=1;		
	
	if(car.right==(-car.right_count))
 		P10=1;		
	
	 

}

void douji_control()
{
 	if(douji.xn<200)
	{	  
	
		douji.x_count++;
		if((douji.x_count )==20)
		{
			P44=1;
		 	douji.x_count=0;
			douji.xn++;
		}
		else if(douji.x==douji.x_count)
	 		P44=0;
		
	}

 	if(douji.yn<200)
	{	  
	
		douji.y_count++;
		if((douji.y_count )==20)
		{
			P35=1;
		 	douji.y_count=0;
			douji.yn++;
		}
		else if(douji.y==douji.y_count)
	 		P35=0;
	}
}

void Time0_Int() interrupt 1  
{
	douji_control();
	car_control();
	start_Timeout500uS();

}

void	main(void)
{
	int i=0,j;
	uart1_rd = 0;
	uart1_wr = 0;
	uart2_rd = 0;
	uart2_wr = 0;
	idle_buf=0;


 	P1M1 &= ~(1<<0),	P1M0 |=  (1<<0);
 	P1M1 &= ~(1<<1),	P1M0 |=  (1<<1);
 	P2M1 &= ~(1<<1),	P2M0 |=  (1<<1);
 	P2M1 &= ~(1<<2),	P2M0 |=  (1<<2);
	P0M1 &= ~(1<<6),	P0M0 |=  (1<<6);	//P 

	P3M1 &= ~(1<<4),	P3M0 |=  (1<<4);
 	P3M1 &= ~(1<<3),	P3M0 |=  (1<<3);
 	P3M1 &= ~(1<<5),	P3M0 |=  (1<<5);

	P4M1 &= ~(1<<4),	P4M0 |=  (1<<4);	//P 
	P10=0;		
//	P13=1;
	pwmd1=1;
//	P22=1;		
	P21=0;	

	P22=0;	
	P11=0;	
	jd=0;
	count=0;
	P44=0;		
	P34=0;		
	P35=0;		
	P33=0;		
	douji.x=0;
	douji.x_count=0;
	douji.y=0;
	douji.y_count=0;
	start_Timeout500uS();
	uart1_init();
	uart2_init();
	PrintString1("com 1 test\n");
	PrintString2("com 2 test\n");

	// while(1);

 
	pwm = PWM_HIGH_MIN;		//pwm 

	for(i=0;i<max_num;i++) 
	{
	 	Receive_CMD[i].receive_status=0;
		Receive_CMD[i].receive_len =0;
	}
	car.right=0;
	car.left=0;

	PrintString1("com 11 test\n");
	
	//Time0_Init();
	jd=4;
	while(1)
	{

		if((car.left==0)&&(car.right==0))
		{
			P34=0;
		}
	 
		if( Receive_CMD[i].receive_status==2)	 
		{
			 cmd_pase(Receive_CMD[i].receive_buf,Receive_CMD[i].receive_len);
			 Receive_CMD[i].receive_status=0;
			 Receive_CMD[i].receive_len=0;
			 PrintString1("eeeee");
			 memset(Receive_CMD[i].receive_buf,0,max_rebuf);
			PrintString2("com 22 test\n");
			//UART1_TxByte(RX1_Buffer[uart1_rd]);
			
			//if(++uart1_rd >= BUF_LENTH)		uart1_rd = 0;
		}
		i++;
		if(i>=max_num)
			i=0;
	}
}



void	uart1_init(void)
{
	PCON |= 0x80;		//UART0 Double Rate Enable
	SCON = 0x50;		//UART0 set as 10bit , UART0 RX enable
	TMOD &= ~(1<<6);		//Timer1 Set as Timer, 12T
	TMOD = (TMOD & ~0x30) | 0x20;	//Timer1 set as 8 bits auto relaod
	TH1 = T1_TimerReload;		//Load the timer
	TR1  = 1;
	ES  = 1;
	EA = 1;
}



/**********************************************/
void UART0_RCV (void) interrupt 4
{
	char datar;
	unsigned char i=0;
	if(RI)
	{
		RI = 0;
		
		 datar=SBUF;
		 receing_data(datar);
		 
	}

	if(TI)
	{
		TI = 0;
		B_TI = 1;
	}
}


/**********************************************/
void	uart2_init(void)
{
	AUXR |=  (1 << 3);		 
	S2CON  = (S2CON & 0x3f) | (1<<6);	
	S2CON |= 1 << 4;		 

	AUXR |=  1 << 4;	//baudrate use BRT
	BRT = BRT_Reload;

	IE2 |=  1;			 
}

