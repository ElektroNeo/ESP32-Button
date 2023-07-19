#include "esp32_button.h"

hw_timer_t *btn_timer = NULL;

//button_t *button_arr [];

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
      btn->mode = LONG_PRESSED;
      if (btn->long_press_fun != NULL)
      {
        btn->long_press_fun();
      }

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
      btn->mode = CLICKED;
      if (btn->click_fun != NULL)
      {
        btn->click_fun();
      }

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

        btn->mode = DOUBLE_CLICKED;

        if (btn->double_click_fun != NULL)
        {
          btn->double_click_fun();
        }

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

void button_init(void (*btn_isr)())
{
  // 1ms timer interrupt
  btn_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(btn_timer, btn_isr, true);
  timerAlarmWrite(btn_timer, 1000, true);
  timerAlarmEnable(btn_timer);
}

void button_add(button_t *btn)
{
  pinMode(btn->pin, INPUT);

  btn->state = IDLE;
  btn->last_val = digitalRead(btn->pin);
}

void button_add_default(button_t *btn, uint8_t pin)
{
  btn->pin = pin;
  btn->debounce_ms = 50;
  btn->double_click_ms = 400;
  btn->long_press_ms = 2000;

  button_add(btn);
}