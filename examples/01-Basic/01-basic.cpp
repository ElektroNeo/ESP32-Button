#include <Arduino.h>
#include <esp32_button.h>

#define BUTTON_1_PIN 33
#define BUTTON_2_PIN 32

button_t button_1;
button_t button_2;

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
  if (button_1.mode || button_2.mode)
  {
    // Print modes: 1- Clicked, 2- Double Clicked 3- Long Pressed
    Serial.print(button_2.mode);
    Serial.print(" - ");
    Serial.println(button_1.mode);

    // Reset modes
    button_1.mode = NONE;
    button_2.mode = NONE;
  }
  
  delay(100);
}