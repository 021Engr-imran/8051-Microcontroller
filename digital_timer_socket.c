#include <reg51.h>
#define lcd_data_port P3  // Use Port 3 as data port
sbit rs = P2^5;           // Register select pin
sbit rw = P2^6;           // Read/Write pin
sbit en = P2^7;           // Enable pin
sbit hours = P1^0;
sbit minus = P1^1;
sbit start = P1^2;
sbit relay = P1^4;

bit count = 0;
int seconds = 59;
int minutes = 0;
int hour = 0;

void delay(unsigned int count) {
    int i, j;
    for (i = 0; i < count; i++)
        for (j = 0; j < 112; j++);
}

void LCD_Command(unsigned char cmd) {
    lcd_data_port = cmd;
    rs = 0;
    rw = 0;
    en = 1;
    delay(1);
    en = 0;
    delay(5);
}

void LCD_Char(unsigned char char_data) {
    lcd_data_port = char_data;
    rs = 1;
    rw = 0;
    en = 1;
    delay(1);
    en = 0;
    delay(10);
}

void LCD_String(unsigned char *str) {
    int i;
    for (i = 0; str[i] != 0; i++) {
        LCD_Char(str[i]);
    }
}

void LCD_Init(void) {   
    delay(20);
    LCD_Command(0x38);
    LCD_Command(0x0C);
    LCD_Command(0x06);
    LCD_Command(0x01);
    LCD_Command(0x80);
}

void LCD_DisplayTime() {
    char number[16];
    number[0] = (hour / 10) + '0';
    number[1] = (hour % 10) + '0';
    number[2] = ':';
    number[3] = (minutes / 10) + '0';
    number[4] = (minutes % 10) + '0';
    number[5] = ':';
    number[6] = (seconds / 10) + '0';
    number[7] = (seconds % 10) + '0';
    number[8] = '\0';
  
    LCD_Command(0xC0); 
    LCD_String(number);
}

void decrementHour() {
    hour--;
    if (hour <= 0) {
        relay = 0;
        while (1) {     
            LCD_Command(0xC0); 
            LCD_String(" Times Up ");
            // Add a break condition or handling to exit the loop
        }
    }
}

void decrementMinutes() {
    minutes--;
    if (minutes <= 0) {
        minutes = 59;
        decrementHour();
    }
}

void decrementTime() {
    seconds--;
    if (seconds <= 0) {
        seconds = 59;
        decrementMinutes();
    }
    LCD_DisplayTime();
    delay(1000);
}

void start_timer() {
    relay = 1; // Activate relay when timer starts
	while(1){
        decrementTime();
	}
}

void set_time() {
    while (1) {
        if (hours == 0) {       
            hour++; 
            if (hour > 5) {
                hour = 0;
            }
            LCD_DisplayTime();
            delay(1000);
        }
        
        if (minus == 0) {
            minutes++;
            if (minutes > 59) {
                minutes = 0;
            }
            LCD_DisplayTime();
            delay(1000);
        }
        
        if (start == 0) {
            start_timer();
        }
    }
}

void main() {
    P1 = 0x0F;  // Set P1.3, P1.2, P1.1, P1.0 high and the rest low
    LCD_Init();
    LCD_Command(0x80); 
    LCD_String(" Digital Timer: ");
    LCD_DisplayTime();
    set_time();
}