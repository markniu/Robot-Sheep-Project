
#include	"PWMn.h"


#define		PCA_ECF				1	 

unsigned int	PWM0_high;					 
unsigned int	PWM0_low;					 
unsigned int	PWM1_high;					 
unsigned int	PWM1_low;				 
unsigned int	CCAP0_tmp,CCAP1_tmp;				 
extern Receive_D  Receive_CMD;
extern car_status   car;

void PWM0_SetHighReg(unsigned int high)
{
	if(high > PWM_HIGH_MAX)		high = PWM_HIGH_MAX;	 
	if(high < PWM_HIGH_MIN)		high = PWM_HIGH_MIN;	 
	CR = 0;					 
	PWM0_high = high;						 
	PWM0_low = PWM_DUTY - high;			 
	CR = 1;					 
}
void PWM1_SetHighReg(unsigned int high)
{
	if(high > PWM_HIGH_MAX)		high = PWM_HIGH_MAX;	 
	if(high < PWM_HIGH_MIN)		high = PWM_HIGH_MIN;	 
	CR = 0;						 
	PWM1_high = high;					 
	PWM1_low = PWM_DUTY - high;				 
	CR = 1;						 
}

void PWMn_init(unsigned int high)
{
	CCON = 0;					 
	IPH |= 0x80;				 
	PPCA = 1;
	CMOD = (PCA_IDLE_DISABLE << 7) | (PCA_SOURCE_SELECT << 1) | PCA_ECF;	 
	CCAPM0 = 0x49;				 
	CCAPM1 = 0x49;				 
	CL = 0;						 
	CH = 0;
	CCAP0_tmp = 100;
	CCAP1_tmp = 100;
	EA = 1;						 
	CR = 1;						 
}

void PCA_interrupt (void) interrupt 7
{
	 
	if(CCF0 == 1)		 
	{
		CCF0 = 0;		 
		
		if(CCP0 == 1)	
		{
			CCAP0_tmp += (PWM_DUTY-abs(car.left)) ;
			if(car.left==0)
			{ //stop
			 	P11=1;		
			  	P22=1;	
				
			}
			if(car.left>0)
			{
			 	P11=~P11;		
			  	P22=0;	
				
			}
			else
			{
		 	 	P11=0;		
		 	 	P22=~P22;	
				 
			}
		 	 
		}
		else			
		{
		 	P11=1;		
		  	P22=1;	
			CCAP0_tmp +=abs(car.left);

		}	
		//CCAP0_tmp += car.left;
		CCAP0L = (unsigned char)CCAP0_tmp;			 
		CCAP0H = (unsigned char)(CCAP0_tmp >> 8);	 
	}

	if(CCF1 == 1)	 
	{
		CCF1 = 0;		 
		if(CCP1 == 1)	
		{
			
			
			if(car.right==0)
			{ // stop
			 	P21=1;		
			 	P10=1;	
			}
			if(car.right>0)
			{
			 	P21=~P21;		
			 	P10=0;	
			}
			else
			{
			 	P21=0;		
			 	P10=~P10;	
			}
			CCAP1_tmp += PWM_DUTY-abs(car.right);
			
		}
		else			
		{
			
			CCAP1_tmp =+abs(car.right);	
		 	P21=1;		
		 	P10=1;	
			
			

		}
		CCAP1L = (unsigned char)CCAP1_tmp;			 
		CCAP1H = (unsigned char)(CCAP1_tmp >> 8);	 



	}
	else if(CF == 1)	 
	{
		CF = 0;			 
	}

}
// else return 1000
int my_atoi(  char * str,int *ret)
{
	int res=0,begin=0,i;
	char  minus=0;
	char ok=0;
	ret[0]=0;
	while(*str!='\0')
	{
		
		if(begin==0&&(('0'<=*str&&*str<='9')|| *str=='-') ) 
		{
			begin=1;
			if(*str == '-')
			{
				minus=1;
				str++;
				 ret[0]++;
			}
		}
		else if( begin==1&&(*str<'0'||*str>'9') )    
		{      
			ok=1;
			break;
		}
		if(begin==1)
			res=res*10+(*str-'0');                       
		 ret[0]++;
		str++;
	}
	
	if(ok==0)
	{
	   ret[0]=0;
	   return max_value;
	}
	return minus? -res : res;
}


int mystrlen(char *s)
{
	int num=0;
	while( *s++) {num++;}
	return (num);
}

void rever(char s[]){  
    int len=strlen(s);  
    int i=0;  
    int j=len-1;  
    char c;  
    while (i<j)  
    {  
        c=s[i];  
        s[i]=s[j];  
        s[j]=c;  
        i++;  
        j--;  
    }  
}  
void my_itoa(int n, char s[])
{  
    int i=0;  
    int sign=0;  
  
    if((sign=n)<0)  
        n=-n;  
  
    do {  
        s[i++]=n%10+'0';  
    } while ((n/=10)>0);  
    if(sign<0)  
        s[i++]='-';  
  
    s[i]='\0';  
  //  rever(s);  
}  
