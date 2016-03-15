 #include<reg51.h>
 #include<intrins.h>
 sbit P10 = P1^0;
 unsigned char KeyVal;
 unsigned Val;
void delay(unsigned int i)
{
	while(i--);
}
 void inf()
 {
 	unsigned char i;
 	unsigned char j;
 	for(j=42;j;j--)
	{
		P10=~P10;
		for(i=30;i!=0;i--){_nop_();}
	}
	P10=1;
 }

 void init()
 {
	 unsigned char i;
 	unsigned int j;
 	for(j=629;j;j--)
	{
		P10=~P10;
		for(i=30;i!=0;i--){_nop_();}
	}
	P10=1;
 }

void wait45000()
{
unsigned int i;
	for(i=3750;i;i--)_nop_();
}
void wait565()
{
unsigned int i;
	for(i=400;i;i--)_nop_();
}
void wait1690()
{
unsigned int i;
	for(i=1350;i;i--)_nop_();	
}
void sendnum(unsigned char Addr,unsigned char Val)
{
unsigned char i=8;
unsigned char addr=Addr;
unsigned char val=Val;
init();
wait45000();
while(i--)
{
	inf();
	if((addr&0x01)==0x01)wait1690();
	else wait565();
	addr>>=1;
}
i=8;
addr=~Addr;
while(i--)
{
	inf();
	if((addr&0x01)==0x01)wait1690();
	else wait565();
	addr>>=1;	
}
i=8;
while(i--)
{
	inf();
	if((val&0x01)==0x01)wait1690();
	else wait565();
	val>>=1;	
}
i=8;
val=~Val;
while(i--)
{
	inf();
	if((val&0x01)==0x01)wait1690();
	else wait565();
	val>>=1;	
}
inf();	
}

unsigned char keyscan(void)
{
  unsigned char keyvalue;
  
  P3 = 0x7F;   //S4 S5 S6 S7
  delay(20);
  switch(P3)
  {
    case 0x7E: keyvalue = 0;break;
    case 0x7D: keyvalue = 4;break;
    case 0x7B: keyvalue = 8;break;
    case 0x77: keyvalue = 12;break;
    default: break;    
  }
  P3 = 0xBF;   //S8 S9 S10 S11
  delay(20);
  switch(P3)
  {
    case 0xBE: keyvalue = 1;break;
    case 0xBD: keyvalue = 5;break;
    case 0xBB: keyvalue = 9;break;
    case 0xB7: keyvalue = 13;break;
    default: break;
  }  
  P3 = 0xDF; //S12 S13 S14 S15
  delay(20);
  switch(P3)
  {
    case 0xDE: keyvalue = 2;break;
    case 0xDD: keyvalue = 6;break;
    case 0xDB: keyvalue = 10;break;
    case 0xD7: keyvalue = 14;break;
    default: break;
  }
  P3 = 0xEF; //S16 S17 S18 S19
  delay(20);
  switch(P3)
  {
    case 0xEE: keyvalue = 3;break;
    case 0xED: keyvalue = 7;break;
    case 0xEB: keyvalue = 11;break;
    case 0xE7: keyvalue = 15;break;
    default: break;  
  }
 // delay(20);
  return keyvalue;
}
 void main()
 {
 P2&=0x1f;
P0=0xff;
P2|=0x80;

P2&=0x1f;
P0=0x01;
P2|=0xa0;
P2&=0x1f;
 while(1)
 {
 KeyVal=keyscan();
 if(Val!=KeyVal)
	 {
	 Val=KeyVal;
	 sendnum(0x00,Val);
	 }
 }	
 }