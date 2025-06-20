#ifndef DS18B20_LIB_H
#define DS18B20_LIB_H
#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define CMD_SKIP_ROM     0xCC
#define CMD_CONVERT_T    0x44
#define CMD_READ_SCRATCH 0xBE

void setup_onewire_pin(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit);
void pull_low(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit);
void let_float(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit);
uint8_t sample_line_state(volatile uint8_t* pin, uint8_t bit);
uint8_t perform_bus_reset(volatile uint8_t* ddr, volatile uint8_t* port, volatile uint8_t* pin, uint8_t bit);
void transmit_single_bit(uint8_t bit_value, volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit);
uint8_t receive_single_bit(volatile uint8_t* ddr, volatile uint8_t* port, volatile uint8_t* pin, uint8_t bit);
void send_byte_data(uint8_t data, volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit);
uint8_t get_byte_data(volatile uint8_t* ddr, volatile uint8_t* port, volatile uint8_t* pin, uint8_t bit);
int16_t get_temperature_reading(volatile uint8_t* ddr, volatile uint8_t* port, volatile uint8_t* pin, uint8_t bit);

#endif