#include <reg51.h>
int scan_key();
int scan_key1();
void display();
void start();
void tochar(unsigned int value);
void onemintdelay();
sbit rs=P3^5;   //Register select (RS)
sbit en=P3^6;   //Enable (EN) pin
sbit r0=P1^0; 
sbit r1=P1^1; 
sbit r2=P1^2; 
sbit r3=P1^3; 
sbit c0=P1^4; 
sbit c1=P1^5; 
sbit c2=P1^6; 
sbit c3=P1^7; 
char clock[]={"CLOCK!"},set[]={"Digital"},hour[]="HOUR 01 to 12",mint[]="MINT 01 to 60";

int count=0,H,M,S=0,hour1[1],mint1[1];


void delay(unsigned int time)  //Time delay function
{
unsigned int i,j;
for(i=0;i< time;i++)
  for(j=0;j< 5;j++);
}

void lcdcmd(unsigned char value)  //Function for sending values to the command register of LCD
{
P2=value;                                 //sending commands on port 2 Means to the lcd command register
P3=0x40;
delay(50);
en=0;
delay(50);
return;
}

void lcddata(unsigned char value)  //Function for sending values to the data register of LCD
{
P2=value;                                //sending data on port 2 Means to the lcd data register 
P3=0x60;
delay(50);
en=0;
delay(50);
return;
}
void lcdint(void)         
{
	P1=0xFF;
P2=0x00;                 
P3=0x00;
     delay(15000);
     lcddata(0x30);
     delay(4500);
     lcddata(0x30);
     delay(300);
     lcddata(0x30);
     delay(650);
lcdcmd(0x38);
delay(50);
lcdcmd(0x0C);
delay(50);
lcdcmd(0x01);
delay(50);
lcdcmd(0x06);
delay(50);
lcdcmd(0x80);
delay(50);
}

void settime()
{
lcdcmd(0x01);			// clear lcd screen 
	lcdcmd(0x80);		// curse foce to begining of 1st line
	while(hour[count]!='\0'){		
	lcddata(hour[count]);
	count++;
	}
	count=0;
	lcdcmd(0xC8); 		 // cursor force to 2nd line 9th position
	while(count!=2){     //This loop uses the scan_key() function to read two digits entered by the user as the hour value
	hour1[count]=scan_key();
	count++;
	}
	H=(hour1[0]*10)+hour1[1];   //Combines the two digits stored in hour1[] into a single integer:
	count=0;
	lcdcmd(0x01);   		// clear lcd screen
	lcdcmd(0x80);				// cursor begin form first line
	while(mint[count]!='\0'){
	lcddata(mint[count]);     //Displays the mint[] string ("MINT 01 to 60"
	count++;
	}
	count=0;
	lcdcmd(0xC8);   		// cursor position
	while(count!=2){
	mint1[count]=scan_key();			// store input in mint1[]
	count++;
	}
	M=(mint1[0]*10)+mint1[1];		//Combines the two digits stored in mint1[] into a single integer (M).
	count=0;
	delay(10000);		//delay 
lcdcmd(0x01); // clear lcd secreen
lcdcmd(0x80);	 // cursor at the 1st line
}

void onemintdelay()   // delay function
{
int count1=0,sec,check;
xyz:
S=2;    // timeer sec start form 2
for(sec=0;sec<59;sec++){
count1=0;
while(count1!=500)
{
TMOD=0x01;
TH0=0xF8;
TL0=0xCC;
TR0=1;
while(!TF0); 		//Wait until Timer0 overflows (indicated by TF0 = 1)
TR0 = 0;				// stop timer 0
TF0 = 0;				// Clear Timer0 overflow flag
count1++;  // Increment the millisecond counter
}
S++;
lcdcmd(0xCB);   // cursor position 2nd line 12th position 
tochar(S);      //onverts the value of S to its ASCII

check=scan_key1();
if(check==1)   // Calls scan_key1() to check if the user presses a specific key
{
start();  // call start function
goto xyz;   
}
}		  
display();  // after completing for loop. call dispay function
}


void main()
{
lcdint();
start();
while(1){
	onemintdelay();}
}
void start()
{
settime();
delay(100);
	count=0;
	lcdcmd(0x01);
	lcdcmd(0x80);
	while(set[count]!='\0'){
	lcddata(set[count]);
	count++;
	}
	count=0;
	lcdcmd(0x89);
while(clock[count]!='\0'){
	lcddata(clock[count]);
	count++;
}
count=0;
lcdcmd(0xC4);
tochar(H);
lcddata(':');
tochar(M);
lcdcmd(0xCB);
tochar(S);

}


void display(){
	count=0;
	lcdcmd(0x01);
	lcdcmd(0x80);
	while(set[count]!='\0'){
	lcddata(set[count]);
	count++;
	}
	count=0;
	lcdcmd(0x89);
while(clock[count]!='\0'){
	lcddata(clock[count]);
	count++;
}
count=0;



if(H==12 && M>58 && S>=59)
{S=1;H=1;M=0;}
if(M>58)
{H++;M=0;S=1;}
if(S>=59)
{M++;S=1;}


lcdcmd(0xC4);
tochar(H);
lcddata(':');
tochar(M);
lcdcmd(0xCB);
tochar(S);
}


int scan_key()
{
	unsigned int c;
	c=22;
	while(!(c==0 && c==1 &&  c==2 && c==3 && c==4 && c==5 && c==6 && c==7 && c==8 && c==9 ))
	{
r0=1;r1=1;r2=1;r3=0;
		if (c1==0 && r3==0)
			{ lcddata('0');delay(10000);P1=0xFF;return c=0;}
r0=1;r1=1;r2=0;r3=1;
	if(c0==0 && r2==0)
		{ lcddata('1');delay(10000);P1=0xFF;return c=1;}
    	if(c1==0 && r2==0)
		{ lcddata('2');delay(10000);P1=0xFF;return c=2;}
		if(c2==0 && r2==0)
		{ lcddata('3');delay(10000);P1=0xFF;return c=3;}
	
r0=1;r1=0;r2=1;r3=1;

	if(c0==0 && r1==0)
		{ lcddata('4');delay(10000);P1=0xFF;return c=4;}
    	if(c1==0 && r1==0)
		{ lcddata('5');delay(10000);P1=0xFF;return c=5;}
	if(c2==0 && r1==0)
		{ lcddata('6');delay(10000);P1=0xFF;return c=6;}
	
r0=0;r1=1;r2=1;r3=1;
	if(c0==0 && r0==0 )
		{lcddata('7');delay(10000);P1=0xFF;return c=7;}
    	if(c1==0 && r0==0)
		{ lcddata('8');delay(10000);P1=0xFF;return c=8;}
	if(c2==0 && r0==0)
		{ lcddata('9');delay(10000);P1=0xFF;return c=9;}
}
P1=0xFF;
return 0;
}

void tochar(unsigned int value)
{
	char tendigit,unitdigit;
	tendigit=value/10;
	lcddata(tendigit+48);
	unitdigit=value%10;
	lcddata(unitdigit+48);
}
int scan_key1()
{
int c;
r0=0;r1=1;r2=1;r3=1;
	if(c0==0 && r0==0 )
		{c=1;}
		else c=0;
		P1=0xFF;
		return c;
}