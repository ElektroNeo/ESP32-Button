#include <Arduino.h>
#include "button.h"

#define BUTTON_PIN 33
#define BUTTON_SMALL_PIN 32

void clicked();
void double_clicked();
void long_pressed();

button_t button = {
    .pin = BUTTON_PIN,
    .debounce_ms = 50,
    .double_click_ms = 400,
    .long_press_ms = 2000,
    .click_fun = &clicked,
    .double_click_fun = &double_clicked,
    .long_press_fun = &long_pressed};

button_t button_small = {
    .pin = BUTTON_SMALL_PIN,
    .debounce_ms = 50,
    .double_click_ms = 400,
    .long_press_ms = 2000,
    .click_fun = &clicked,
    .double_click_fun = &double_clicked,
    .long_press_fun = &long_pressed};

void IRAM_ATTR button_isr()
{
  button_update(&button);
  button_update(&button_small);
}

void setup()
{
  Serial.begin(115200);

  button_add(&button);
  button_add(&button_small);

  button_init(&button_isr);
}

void loop()
{
  delay(100);
}

void clicked()
{
  if (button.mode == CLICKED)
  {
    Serial.println("Clicked 1!");
    button.mode = NONE;
  }
  if (button_small.mode == CLICKED)
  {
    Serial.println("Clicked 2!");
    button_small.mode = NONE;
  }
}

void double_clicked()
{
  if (button.mode == DOUBLE_CLICKED)
  {
    Serial.println("Double Clicked 1!");
    button.mode = NONE;
  }
  if (button_small.mode == DOUBLE_CLICKED)
  {
    Serial.println("Double Clicked 2!");
    button_small.mode = NONE;
  }
}

void long_pressed()
{
  if (button.mode == LONG_PRESSED)
  {
    Serial.println("Long Pressed 1!");
    button.mode = NONE;
  }
  if (button_small.mode == LONG_PRESSED)
  {
    Serial.println("Long Pressed 2!");
    button_small.mode = NONE;
  }
}