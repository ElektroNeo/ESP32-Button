#include <Arduino.h>
#include "button.h"

#define BUTTON_PIN 33

button_t button = {
    .pin = BUTTON_PIN,
    .debounce_ms = 50,
    .double_click_ms = 400,
    .long_press_ms = 2000,
    .click_fun = &clicked,
    .double_click_fun = &double_clicked,
    .long_press_fun = &long_pressed};

void IRAM_ATTR button_isr()
{
  button_update(&button);
}

void clicked()
{
  Serial.println("Clicked!");
}

void double_clicked()
{
  Serial.println("Double Clicked!");
}

void long_pressed()
{
  Serial.println("Long Pressed!");
}

void setup()
{
  Serial.begin(115200);
  button_init(&button, &button_isr);
}

void loop()
{
  delay(100);
}