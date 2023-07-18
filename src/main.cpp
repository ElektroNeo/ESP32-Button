#include <Arduino.h>

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

#define BUTTON_PIN 33

hw_timer_t *btn_timer = NULL;

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
  // Private
  button_state state;
  int8_t val;
  int8_t last_val;
  uint16_t debounce_timer;
  uint16_t double_click_timer;
  uint16_t long_press_timer;
} button_t;

button_t button = {
    .pin = BUTTON_PIN,
    .debounce_ms = 50,
    .double_click_ms = 400,
    .long_press_ms = 2000,
    .click_fun = &clicked,
    .double_click_fun = &double_clicked,
    .long_press_fun = &long_pressed};

void button_update(button_t *btn)
{
  // Check button state
  btn->val = digitalRead(btn->pin);

  switch (btn->state)
  {
  case IDLE:
    if (btn->val != btn->last_val)
    {
      // Update last value
      btn->last_val = btn->val;

      btn->debounce_timer = 0;

      btn->state = DEBOUNCE_WAIT;
    }

    break;
  case DEBOUNCE_WAIT:
    // Increase debaunce timer
    btn->debounce_timer++;

    // Check for debounce limit
    if (btn->debounce_timer > btn->debounce_ms)
    {
      if (btn->val == btn->last_val)
      {
        btn->long_press_timer = 0;

        btn->state = CLICK_WAIT;
      }
      else
      {
        // Update last value
        btn->last_val = btn->val;
        btn->state = IDLE;
      }
    }

    break;
  case CLICK_WAIT:
    // Increase long press timer
    btn->long_press_timer++;

    // Check for release
    if (btn->val != btn->last_val)
    {
      // Update last value
      btn->last_val = btn->val;

      btn->double_click_timer = 0;

      btn->state = DOUBLE_CLICK_WAIT;
    }

    // Check for debounce limit
    if (btn->long_press_timer > btn->long_press_ms)
    {
      btn->long_press_fun();

      btn->state = RELEASE_WAIT;
    }

    break;

  case DOUBLE_CLICK_WAIT:
    // Increase long press timer
    btn->double_click_timer++;

    // Check for release
    if (btn->val != btn->last_val)
    {
      // Update last value
      btn->last_val = btn->val;

      btn->debounce_timer = 0;

      btn->state = DOUBLE_CLICK_DEBOUNCE_WAIT;
    }

    // Check for debounce limit
    if (btn->double_click_timer > btn->double_click_ms)
    {
      btn->click_fun();

      btn->state = IDLE;
    }

    break;
  case DOUBLE_CLICK_DEBOUNCE_WAIT:
    // Increase debaunce timer
    btn->debounce_timer++;

    // Check for debounce limit
    if (btn->debounce_timer > btn->debounce_ms)
    {
      if (btn->val == btn->last_val)
      {

        btn->double_click_fun();
        btn->state = RELEASE_WAIT;
      }
      else
      {
        // Update last value
        btn->last_val = btn->val;
        btn->state = IDLE;
      }
    }

    break;
  case RELEASE_WAIT:
    // Check for release
    if (btn->val != btn->last_val)
    {

      // Update last value
      btn->last_val = btn->val;

      btn->debounce_timer = 0;

      btn->state = RELEASE_DEBOUNCE_WAIT;
    }

    break;
  case RELEASE_DEBOUNCE_WAIT:
    // Increase debaunce timer
    btn->debounce_timer++;

    // Check for debounce limit
    if (btn->debounce_timer > btn->debounce_ms)
    {
      // Update last value
      btn->last_val = btn->val;
      btn->state = IDLE;
    }

    break;
  default:
    break;
  }
}

void IRAM_ATTR button_isr()
{
  button_update(&button);
}

void button_init(button_t *button)
{
  pinMode(button->pin, INPUT);

  button->state = IDLE;
  button->last_val = digitalRead(button->pin);

  // 1ms timer interrupt
  btn_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(btn_timer, &button_isr, true);
  timerAlarmWrite(btn_timer, 1000, true);
  timerAlarmEnable(btn_timer);
}

void setup()
{
  Serial.begin(115200);
  button_init(&button);
}

void loop()
{
  // Serial.print(button.debounce_timer);
  // Serial.print(" - ");
  // Serial.println(btn_stt);
  delay(100);
}