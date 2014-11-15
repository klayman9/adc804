
#ifndef WORK12_H
#define WORK12_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <reg51.h>

#define DISPLAY_OUT P2// P2
#define ADC_OUT P1
#define LOGIC_DATA P0

sbit LOGIC_DATA0 = P0 ^ 0;
sbit LOGIC_DATA1 = P0 ^ 1;
sbit LOGIC_DATA2 = P0 ^ 2;
sbit LOGIC_DATA3 = P0 ^ 3;
sbit LOGIC_DATA4 = P0 ^ 4;
sbit LOGIC_DATA5 = P0 ^ 5;
sbit LOGIC_DATA6 = P0 ^ 6;
sbit LOGIC_DATA7 = P0 ^ 7;

sbit CS  = P3 ^ 0; //
sbit CLK = P3 ^ 1; //
sbit DO  = P3 ^ 2; //

sbit RS = P3 ^ 5; // Reset.
sbit RW = P3 ^ 6; // Write.
sbit E  = P3 ^ 7; // Enable.

#define LOGIC_INPUT_NUMBER 8
#define ANALOG_INPUT_NUMBER 17
#define ADC_COUNT 11
#define S_COUNT 2

code char msg1[] = "Novikov          ";
code char msg2[] = "Sergey           ";
code char msg3[] = "Igorevich        ";
code char msg4[] = "VMKSS 0904       ";
volatile char str[18] = {0}; // Buffer for message.
volatile char buff[18] = {0}; // Buffer for message.

void initialyze(); // Main initialyze.
void display_initialyze(); // Display initialyze.
void timer_initialyze(); // Timer0 initialyze.

void timer_start(); // Timer0 start.

void command(unsigned char command); //Command to display.
void print(unsigned char *line); // Print string to display.
void out(unsigned char output); // Print to display.

unsigned char get_analog_value(); // For analog function.

char f1(); // Logic function.
char f2(); // Analog function.

void delay(char count);
void shift();
void conversion();
void roll_right();

volatile char seconds_count; // Seconds.
volatile char delay_count; // 0.01 seconds
volatile char timer_tick_count_one; // Timer tick = 0.01 seconds.
volatile char timer_tick_count_two; // Timer tick = 0.01 seconds.
volatile char do_conversion; // Start AD conversion flag.
volatile char do_shift; // Message shift flag.
volatile char current_value;
volatile char time;

#endif