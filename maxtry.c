// Author: aathil ahmeth

// CONFIG
#pragma config FOSC = EXTRC    // Oscillator Selection bits (EXTRC oscillator)
#pragma config WDTE = OFF      // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF     // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF     // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF       // Low-Voltage (Single-Supply) In-Circuit Serial Programming Disable bit (RB3 is digital I/O)
#pragma config CPD = OFF       // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF       // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF        // Flash Program Memory Code Protection bit (Code protection off)
#include <xc.h>

#define _XTAL_FREQ 20000000 // Define oscillator frequency for delay

// Pin definitions
#define DIN PORTBbits.RB0
#define CS     PORTBbits.RB1
#define CLK    PORTBbits.RB2

// Function prototypes
void initialize_ports(void);
void init_max6952();
void send_command(uint8_t address, uint8_t data);
void test_max6952();

void initialize_ports(void) {
    TRISB = 0x00; // Configure PORTB as output
    PORTB = 0x00; // Clear PORTB
}

void send_command(uint8_t address, uint8_t data) {
    CS = 0; // Enable device
    for (int i = 0; i < 8; i++) {
        DIN = (address & 0x80) >> 7;
        CLK = 0;
        __delay_us(1);
        CLK = 1;
        address <<= 1;
    }
    for (int i = 0; i < 8; i++) {
        DIN = (data & 0x80) >> 7;
        CLK = 0;
        __delay_us(1);
        CLK = 1;
        data <<= 1;
    }
    CS = 1; // Disable device
}
void init_max6952() {
    send_command(0x04, 0x01); // Exit shutdown mode
    send_command(0x03, 0x01); // Enable all four digits
    send_command(0x01, 0x0F); // Max intensity for digits 0 and 1
    send_command(0x02, 0xF0); // Max intensity for digits 2 and 3   
}

void test_max6952() {
    send_command(0x07, 0x01); // Display test mode (all LEDs on)
    __delay_ms(1000);
    send_command(0x07, 0x00); // Exit test mode
}


void main(void) {
    initialize_ports();
    init_max6952();
    
    uint8_t display_data1[] = {0x41, 0x32, 0x33, 0x34};
    uint8_t display_data2[] = {0x42, 0x33, 0x32, 0x31};
    
        for (int i = 0; i < sizeof(display_data1) / sizeof(display_data1[0]); i++) {
            send_command(0x20 + i, display_data1[i]);
        }
        __delay_us(10000000);

        for (int j = 0; j < 4; j++) {
            send_command(0x20 + j, display_data2[j]);
        }
        __delay_us(10000000);
    
}
