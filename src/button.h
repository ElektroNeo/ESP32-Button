#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

typedef enum
{
  IDLE,
  DEBOUNCE_WAIT,
  CLICK_WAIT,
  DOUBLE_CLICK_WAIT,
  DOUBLE_CLICK_DEBOUNCE_WAIT,
  RELEASE_WAIT,
  RELEASE_DEBOUNCE_WAIT
} button_state;

typedef enum
{
  NONE,
  CLICKED,
  DOUBLE_CLICKED,
  LONG_PRESSED
} button_mode;

typedef struct
{
  // Public
  uint8_t pin;
  uint16_t debounce_ms;
  uint16_t double_click_ms;
  uint16_t long_press_ms;
  void (*click_fun)();
  void (*double_click_fun)();
  void (*long_press_fun)();
  uint8_t mode;
  // Private
  button_state state;
  int8_t val;
  int8_t last_val;
  uint16_t debounce_timer;
  uint16_t double_click_timer;
  uint16_t long_press_timer;
} button_t;

void button_init(void (*btn_isr)());
void button_add(button_t *btn);
void button_add_default(button_t *btn, uint8_t pin);
void button_update(button_t *btn);

#endif