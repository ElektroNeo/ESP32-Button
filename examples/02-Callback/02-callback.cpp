#include <Arduino.h>
#include <esp32_button.h>

#define BUTTON_1_PIN 33
#define BUTTON_2_PIN 32

void clicked();
void double_clicked();
void long_pressed();

button_t button_1 = {
    .click_fun = &clicked,
    .double_click_fun = &double_clicked,
    .long_press_fun = &long_pressed};

button_t button_2 = {
    .click_fun = &clicked,
    .double_click_fun = &double_clicked,
    .long_press_fun = &long_pressed};

void IRAM_ATTR button_isr()
{
  button_update(&button_1);
  button_update(&button_2);
}

void setup()
{
  Serial.begin(115200);

  button_add_default(&button_1, BUTTON_1_PIN);
  button_add_default(&button_2, BUTTON_2_PIN);

  button_init(&button_isr);
}

void loop()
{
  delay(100);
}

void clicked()
{
  if (button_1.mode == CLICKED)
  {
    Serial.println("Clicked 1!");
    button_1.mode = NONE;
  }
  if (button_2.mode == CLICKED)
  {
    Serial.println("Clicked 2!");
    button_2.mode = NONE;
  }
}

void double_clicked()
{
  if (button_1.mode == DOUBLE_CLICKED)
  {
    Serial.println("Double Clicked 1!");
    button_1.mode = NONE;
  }
  if (button_2.mode == DOUBLE_CLICKED)
  {
    Serial.println("Double Clicked 2!");
    button_2.mode = NONE;
  }
}

void long_pressed()
{
  if (button_1.mode == LONG_PRESSED)
  {
    Serial.println("Long Pressed 1!");
    button_1.mode = NONE;
  }
  if (button_2.mode == LONG_PRESSED)
  {
    Serial.println("Long Pressed 2!");
    button_2.mode = NONE;
  }
}