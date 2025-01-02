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
char clock[]={"CLOCK!"},hour[]="HOUR 01 to 12",mint[]="MINT 01 to 60",set[]={"digital "};

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
	P1=0xFF; //Sets all pins of Port 1 as inputs. The value 0xFF in hexadecimal corresponds to 11111111 in binary, 
						//enabling all the bit
P2=0x00;         //Sets all pins of Port 2 as outputs with a low (0) state        
P3=0x00;   		//Sets all pins of Port 3 as outputs with a low state.
     delay(15000);  
     lcddata(0x30);  ////Sends the value 0x30 to the LCD as initialization data. 
									//This is a standard command for setting the LCD in 8-bit mode
     delay(4500);
     lcddata(0x30);
     delay(300);
     lcddata(0x30);
     delay(650);
lcdcmd(0x38);  //Configures the LCD to use 2-line mode with 5x7 dot format (standard).
delay(50);
lcdcmd(0x0C);		//Turns on the display and disables the cursor.
delay(50);
lcdcmd(0x01);		//Clears the display and resets the cursor to the home position (top-left corner).
delay(50);
lcdcmd(0x06);		//Configures the entry mode to increment the cursor after each character entry
delay(50);
lcdcmd(0x80);		//Sets the cursor to the first position of the first line (line addressing starts at 0x80 for most LCDs).
delay(50);
}

void settime()
{
lcdcmd(0x01);
	lcdcmd(0x80);
	while(hour[count]!='\0'){
	lcddata(hour[count]);
	count++;
	}
	count=0;
	lcdcmd(0xC8);
	while(count!=2){
	hour1[count]=scan_key();
	count++;
	}
	H=(hour1[0]*10)+hour1[1];
	count=0;
	lcdcmd(0x01);
	lcdcmd(0x80);
	while(mint[count]!='\0'){
	lcddata(mint[count]);
	count++;
	}
	count=0;
	lcdcmd(0xC8);
	while(count!=2){
	mint1[count]=scan_key();
	count++;
	}
	M=(mint1[0]*10)+mint1[1];
	count=0;
	delay(10000);
lcdcmd(0x01);
lcdcmd(0x80);	
}

void onemintdelay()
{
int count1=0,sec,check;
xyz:
S=1;
for(sec=0;sec<59;sec++){
count1=0;
while(count1!=500)
{
TMOD=0x01;
TH0=0xF8;
TL0=0xCC;
TR0=1;
while(!TF0);
TR0 = 0;
TF0 = 0;
count1++;
}
S++;
lcdcmd(0xCB);
tochar(S);

check=scan_key1();
if(check==1)
{
start();
goto xyz;
}
}		  
display();
}


void main()
{
lcdint();
start();
while(1){
	onemintdelay();
}
}
void start()
{
settime();
delay(100);
	count=0;
	lcdcmd(0x01); // clear cursor
	lcdcmd(0x80); // 1st line
	while(set[count]!='\0'){
	lcddata(set[count]);  //Sends the character at the current index to the LCD for display
	count++;              // increment to move
	}
	count=0;
	lcdcmd(0x89);				// move cursor 
while(clock[count]!='\0'){
	lcddata(clock[count]);
	count++;
}
count=0;
lcdcmd(0xC4);	//move cursor to 2nd line c4 colmn  Line 1= 0x80 - 0x8F Line 2= 0xC0 - 0xCF
tochar(H);			//Converts the integer value of H (hours) to a character( ASCII) and sends it to the LCD.
lcddata(':');
tochar(M);
lcdcmd(0xCB);    //move cursor to 12th position of 2nd line
tochar(S);

}


void display(){
	count=0;
	lcdcmd(0x01);    // clear display
	lcdcmd(0x80);		// Force cursor to beginning to 1st line
	while(set[count]!='\0'){
	lcddata(set[count]);
	count++;
	}
	count=0;
	lcdcmd(0x89);					//sets the cursor position to the beginning of the first line.
while(clock[count]!='\0'){
	lcddata(clock[count]);
	count++;
}
count=0;



if(H==12 && M>=60 && S>=60)
{
	S=1;
	H=1;
	M=1;
	}
if(M>=60)
{
	H++;
	M=1;
	S=1;
	}
if(S>=60)
{
	M++;
	S=1;
}


lcdcmd(0xC4); 		//Moves the LCD cursor to a specific position.  Line 1= 0x80 - 0x8F Line 2= 0xC0 - 0xCF
									//The command 0xC4 corresponds to the fifth position (memory address 0xC4) on the second line of the LCD.
tochar(H);			//H to its ASCII  
lcddata(':');
tochar(M);
lcdcmd(0xCB);   //Moves the LCD cursor to 12th position on the second line.
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
	char tendigit,unitdigit;  // convert numercial code in to ASCII code
	tendigit=value/10;
	lcddata(tendigit+48);   //ASCII arithmetic (+48) to translate numbers into characters
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
