#include <reg51.h>
#include<intrins.h>
 unsigned char NUM[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
 sbit P11=P1^1;
 unsigned int count;
 unsigned char VL;
 unsigned char VH;
 unsigned char VAL;
 void delay(unsigned int i)
 {
 while(i--);
 }
 
  void Display_One_Digit(unsigned char loc,unsigned char val);
  void Display_Num(unsigned char val);
  void main()
  {
  	unsigned char i;
	unsigned char time_slice=0;
	EA=1;
	
	TMOD=0X01;
	TR0=1;
	
	P2&=0x1f;
	P0=0xff;
	P2|=0x80;

	P2&=0x1f;
	P0=0x01;
	P2|=0xa0;
	P2&=0x1f;
	while(1){
	delay(100);
	while((time_slice==0)&&(P11==0))
	{
		i=32;
		TL0=TH0=0;
		delay(10);
		while(!P11);
		if((TH0*256+TL0)>5000)
		{
			TL0=TH0=0;
			delay(10);
			do{	
			while(P11&&((TH0*256+TL0)<6000));
			if((TH0*256+TL0)>6000)goto loop; //判断是不是由于超时造成的退出循环
			delay(1);
			}while(P11);	//保证确实不是因为干扰毛刺下来的
			while(i--)
			{
				
				TL0=TH0=0;
				while(!P11);
				do
				{
				while(P11&&((TH0*256+TL0)<3000));
				if((TH0*256+TL0)>3000)goto loop;
				count=(TH0*256+TL0);
				delay(1);
				}while(P11);	 //保证确实不是因为干扰毛刺下来的	
				if(i<16)
				{
					if(i>7)
					{
						VH>>=1;
						if((count)>2000)VH|=0x80;
					}
					else
					{
						VL>>=1;
						if((count)>2000)VL|=0x80;
					}	
				}
			}
			if((VH^VL)==0xff)VAL=VH;
		}
	}
	
loop:
	time_slice=1;
	while(time_slice==1)
	{
		if(P11==0)break;
		Display_Num(VAL);
		delay(50);
	}
	time_slice=0;
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



