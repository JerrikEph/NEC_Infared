#include <reg51.h>
#include<intrins.h>
 unsigned char NUM[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
 sbit P32=P3^2;
// bit Flag1;
 unsigned int count;
 unsigned char VL;
 unsigned char VH;
 unsigned char VAL;
 unsigned char Tbusy,Rbusy;
 void delay(unsigned int i)
 {
 while(i--);
 }
 
  void Display_One_Digit(unsigned char loc,unsigned char val);
  void Display_Num(unsigned char val);
  void main()
  {
  
	IT0=1;/*enable interrupt 0*/
	EX0=1;
	EA=1;
	
	TMOD=0X01;
	TR0=1;
	//ET0=1;
	
	P2&=0x1f;
	P0=0xff;
	P2|=0x80;

	P2&=0x1f;
	P0=0x01;
	P2|=0xa0;
	P2&=0x1f;
	while(1)
	{
		Display_Num(VAL);
		delay(50);
	}
  }
  
  void Display_One_Digit(unsigned char loc,unsigned char val)
{
	P2&=0x1f;
	P2|=0xef;
	P0=val;
	P2&=0x1f;

	switch(loc)
	{
		case 8:P0=0x01;break;
		case 7:P0=0x02;break;
		case 6:P0=0x04;break;
		case 5:P0=0x08;break;
		case 4:P0=0x10;break;
		case 3:P0=0x20;break;
		case 2:P0=0x40;break;
		case 1:P0=0x80;break;
	}
	
	P2|=0xc0;
	delay(100);		/*************/
	P0=0x00;
	P2*=0x1f;
}
void Display_Num(unsigned char val)
{
	if((val/100)!=0)Display_One_Digit(3,NUM[val/100]);
	if(((val/100)!=0)||((val/10)!=0))Display_One_Digit(2,NUM[(val/10)%10]);
	Display_One_Digit(1,NUM[val%10]);	
}

void exint0() interrupt 0
{
	int i=32;//42;/*这里的数字代表地址码和数据码的总位数*/
	EX0=0;
	TL0=TH0=0;
	//TF0=0;//clear timer0 inturrupt flag
	//ET0=1;//enable timer0 inturrupt
	//Flag1=1;
	delay(10);
	//VAL=1;
	while(!P32);
	
	//VAL=(TH0*256+TL0)/1000;
	if((TH0*256+TL0)>5000)
	{
		
		delay(10);
		
		while(P32&&((TH0*256+TL0)<15000));
		if((TH0*256+TL0)>15000)goto loop;
		delay(15);
		while(P32&&((TH0*256+TL0)<15000));
		if((TH0*256+TL0)>15000)goto loop;
		delay(15);
		while(!P32);
		TL0=TH0=0;
		if(!P32)
		{
		    while(!P32);
			TL0=TH0=0;
		}
		while(i--)
		{
			
			while(!P32);
			delay(15);
			while(!P32);
			delay(15);
			while(P32&&((TH0*256+TL0)<15000));
			if((TH0*256+TL0)>15000)goto loop;
			count=(TH0*256+TL0);
			delay(15);
			if(P32&&((TH0*256+TL0)<15000)){
			    while(P32&&((TH0*256+TL0)<15000));
				if((TH0*256+TL0)>15000)goto loop;
				count=(TH0*256+TL0);
			}
			TL0=TH0=0;
			if(i<16)
			{
				if(i>7)
				{
					VH>>=1;
					if((count)>2000)
						VH|=0x80;
				}
				else
				{
					VL>>=1;
					if((count)>2000)VL|=0x80;
				}
				
			}
		}
		//VL=0xff;
		if((VH^VL)==0xff)VAL=VH;
		//Flag1=0;
		//while(!P32);
	}
	//ET0=0;//disable timer0 inturrupt
loop:
	IE0=0;
	EX0=1;
}

/*void tm0_isr() interrupt 1
{
	VAL=7;
	if(Flag1!=0)Flag1=0;
}  */
