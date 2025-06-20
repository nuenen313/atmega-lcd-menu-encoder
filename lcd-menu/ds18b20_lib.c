#include "ds18b20_lib.h"

void setup_onewire_pin(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit) {
	*ddr &= ~(1 << bit);
	*port |= (1 << bit);
}
void pull_low(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit) {
	*ddr |= (1 << bit);
	*port &= ~(1 << bit);
}
void let_float(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit) {
	*ddr &= ~(1 << bit);
	*port |= (1 << bit);
}

uint8_t sample_line_state(volatile uint8_t* pin, uint8_t bit) {
	uint8_t pin_mask = (1 << bit);
	uint8_t pin_reading = *pin & pin_mask;
	
	if (pin_reading != 0) {
		return 1;
		} else {
		return 0;
	}
}

uint8_t perform_bus_reset(volatile uint8_t* ddr, volatile uint8_t* port, volatile uint8_t* pin, uint8_t bit) {
	uint8_t device_detected;
	pull_low(ddr, port, bit);
	_delay_us(480);
	let_float(ddr, port, bit);
	_delay_us(70);
	device_detected = !sample_line_state(pin, bit);
	_delay_us(410);
	return device_detected;
}

void transmit_single_bit(uint8_t bit_value, volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit) {
	pull_low(ddr, port, bit);
	if (bit_value != 0) {
		_delay_us(6);
		let_float(ddr, port, bit);
		_delay_us(64);
		} else {
		_delay_us(60);
		let_float(ddr, port, bit);
		_delay_us(10);
	}
}

uint8_t receive_single_bit(volatile uint8_t* ddr, volatile uint8_t* port, volatile uint8_t* pin, uint8_t bit) {
	uint8_t bit_result;
	pull_low(ddr, port, bit);
	_delay_us(6);
	let_float(ddr, port, bit);
	_delay_us(9);
	bit_result = sample_line_state(pin, bit);
	_delay_us(55);
	return bit_result;
}

void send_byte_data(uint8_t data, volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit) {
	uint8_t bit_index = 0;
	while (bit_index < 8) {
		transmit_single_bit(data & 0x01, ddr, port, bit);
		data >>= 1;
		bit_index++;
	}
}

uint8_t get_byte_data(volatile uint8_t* ddr, volatile uint8_t* port, volatile uint8_t* pin, uint8_t bit) {
	uint8_t received_data = 0;
	uint8_t bit_counter = 0;
	while (bit_counter < 8) {
		received_data >>= 1;
		if (receive_single_bit(ddr, port, pin, bit)) {
			received_data |= 0x80;
		}
		bit_counter++;
	}
	return received_data;
}

int16_t get_temperature_reading(volatile uint8_t* ddr, volatile uint8_t* port, volatile uint8_t* pin, uint8_t bit) {
	if (!perform_bus_reset(ddr, port, pin, bit)) return -100;
	
	send_byte_data(CMD_SKIP_ROM, ddr, port, bit);
	send_byte_data(CMD_CONVERT_T, ddr, port, bit);
	uint16_t wait_counter = 0;
	while (wait_counter < 750) {
		_delay_ms(1);
		if (sample_line_state(pin, bit)) break;
		wait_counter++;
	}
	
	if (!perform_bus_reset(ddr, port, pin, bit)) return -1000;
	send_byte_data(CMD_SKIP_ROM, ddr, port, bit);
	send_byte_data(CMD_READ_SCRATCH, ddr, port, bit);
	uint8_t low_byte = get_byte_data(ddr, port, pin, bit);
	uint8_t high_byte = get_byte_data(ddr, port, pin, bit);
	return (high_byte << 8) | low_byte;
}
