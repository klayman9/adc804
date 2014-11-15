
#include <work12.h>

void delay(char count)
{
	// Check bad value.
	if (count <= 0) {
		return;
	}
	
	delay_count = count;
	while (delay_count) {
		// Wait.
	}
}

void timer_start()
{
	TR0 = 1; // Timer start bit on.	
}

unsigned char get_analog_value()
{
	unsigned char i, value=0;
	DO = 1;
	CS = 0;			
	CLK = 1;
	CLK = 0;
	for (i = 0; i < 11; i++) {
		delay(2);
		CLK = 1;
		if(i >= 2 && i <= 9) {
			value <<= 1;
			value |= DO;
		}	
		delay(1);
		CLK = 0;
	}	
	CS = 1;	
	if (do_shift) {
		shift();
		do_shift = 0;
	}


	return value; 
}

char f1()
{
	return (LOGIC_DATA7)  &
		   (LOGIC_DATA6)  &
		   (!LOGIC_DATA5) ^
     	   (!LOGIC_DATA4) | 
	       (!LOGIC_DATA3) | 
	       (!LOGIC_DATA2) & 
	       (LOGIC_DATA1)  ^ 
	       (LOGIC_DATA0);
}

char f2()
{
	int summ = 0, right = 0, i;
	for (i = 0; i < ANALOG_INPUT_NUMBER - 1; i += 2) {
		if (i >= 15) i |= 0xF; 
		ADC_OUT = i;
		summ += get_analog_value();
	}

	ADC_OUT = 0x2F;
	right = get_analog_value();
	right += right/2;
	
	if (summ < right) {
		return 1;
	} else {
		return 0;
	}
}

void initialyze()
{
	P1 = 0x00;
	DISPLAY_OUT = 0x00;

	P3 = 0x00; 
	EA = 1; // All interrupt on.
	DO = 1;
	current_value = -1;
}

void display_initialyze()
{
	command(0x0C);
}

void command(unsigned char command)
{
	DISPLAY_OUT = command;
	E = 1; // Display enable.
	delay(4);
	E = 0; 
	delay(4); // 40 ms.
}

void out(unsigned char output)
{
	DISPLAY_OUT = output;
	E  = 1; 
	RS = 1;
	delay(0); // 0 ms.
	E  = 0;
	RS = 0;
}

void print(unsigned char *line)
{
	while(*line) {
		out(*line);
		line++;
	}
}

void timer_initialyze()
{
	// Initialyze global timer variables.
	seconds_count = 0; 
	timer_tick_count_one = 0;
	timer_tick_count_two = 0;
	
	// Initialyze timer0 bits.
 	TH0 = 0xDB;
	TL0 = 0xF2;
	TMOD |= 0x1;
	ET0 = 1;
}

void R0Isr(void) interrupt 1 using 1
{
	TF0 = 0;
	
	// For delay function.
	if (delay_count > 0) {
		delay_count--;
	}
	
	// For conversion flag.
	timer_tick_count_one++;
	
	// For shift flag.
	timer_tick_count_two++;
	
	if (timer_tick_count_one >= 100) {
		timer_tick_count_one = 0;
		time++;
		if (time >= 3) {
			time = 0;
			do_conversion = 1; 
		}
	}
	
	if (timer_tick_count_two >= 50) {
		timer_tick_count_two = 0;
		// Set shift flag.
		
		do_shift = 1;
	}
	
	TH0 = 0xDB;
	TL0 = 0xF2;
}

void roll_right()
{
	char i;
	for (i = 0; i < strlen(str); i++) {
        if (i == 0) {
            str[0] = buff[strlen(buff) - 1];
        } else {
            str[i] = buff[i - 1];
        }
    }
	strcpy(buff, str);
}

void shift()
{
	command(0x01);
	roll_right();
	print(str);
}

void conversion()
{
	char res = 0;
	
	res = f1();
	res <<= 1;
	res |= f2();
	
	if (res != current_value) {
		current_value = res;
	} else {
		return;
	}

	if (res == 0) {
		strcpy(str, msg1);
		strcpy(buff, msg1);
	}
	
	if (res == 1) {
		strcpy(str, msg2);
		strcpy(buff, msg2);
	}
	
	if (res == 2) {
		strcpy(str, msg3);
		strcpy(buff, msg3);
	}
	
	if (res == 3) {
		strcpy(str, msg4);
		strcpy(buff, msg4);
	}
}

int main()
{
	strcpy(str, msg1);
	strcpy(buff, msg1);
	
	initialyze();
	timer_initialyze();
	timer_start();
	display_initialyze();
	
	print(str);

	while (1) {
		
		if (do_shift) {
			shift();
			do_shift = 0;
		}

		if (do_conversion) {
			conversion();
			do_conversion = 0;
		}
	}
	return 0;
}