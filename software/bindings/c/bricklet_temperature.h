/*************************************************************
 * This file was automatically generated on 23.08.2011.      *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generator git on tinkerforge.com                   *
 *************************************************************/

#ifndef BRICKLET_TEMPERATURE_H
#define BRICKLET_TEMPERATURE_H

#include "ip_connection.h"

typedef Device Temperature;

#define TEMPERATURE_CALLBACK_TEMPERATURE 8
#define TEMPERATURE_CALLBACK_TEMPERATURE_REACHED 9

void temperature_create(Temperature *temperature, const char *uid);

int temperature_get_temperature(Temperature *temperature, int16_t *ret_temperature);
int temperature_set_temperature_callback_period(Temperature *temperature, uint32_t period);
int temperature_get_temperature_callback_period(Temperature *temperature, uint32_t *ret_period);
int temperature_set_temperature_callback_threshold(Temperature *temperature, char option, int16_t min, int16_t max);
int temperature_get_temperature_callback_threshold(Temperature *temperature, char *ret_option, int16_t *ret_min, int16_t *ret_max);
int temperature_set_debounce_period(Temperature *temperature, uint32_t debounce);
int temperature_get_debounce_period(Temperature *temperature, uint32_t *ret_debounce);
int temperature_temperature(Temperature *temperature, int16_t *ret_temperature);
int temperature_temperature_reached(Temperature *temperature, int16_t *ret_temperature);

void temperature_register_callback(Temperature *temperature, uint8_t cb, void *func);

#endif