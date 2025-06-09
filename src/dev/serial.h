#ifndef SERIAL_H
#define SERIAL_H

void dev_serial_init();
void dev_serial_send(uint8_t *data, uint16_t len);
__attribute__((weak)) void dev_serial_on_receive(uint8_t *data, uint16_t len);

#endif