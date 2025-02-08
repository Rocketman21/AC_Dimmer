#include <EEPROM.h>

unsigned int boot_count = 0;

int boot_count_eeprom_addr = 0;
int value_eeprom_addr = sizeof(boot_count);

int min_value = 50;
int max_value = 250;
int value = min_value; // Начальный уровень диммирования от 0 до 255

int dim_pin = 4; // Выход диммера на симистор

int brightness_pin_1 = 5;
int brightness_pin_2 = 6;
int brightness_pin_3 = 7;
int brightness_pin_4 = 8;
int brightness_pin_adjustable = 9;

bool should_increase = true;

unsigned long brightness_timing;
unsigned long eeprom_timing;

void setup() {
  EEPROM.get(boot_count_eeprom_addr, boot_count);
  EEPROM.get(value_eeprom_addr, value);

  if (value < min_value) value = min_value;

  EEPROM.put(0, boot_count++);

  pinMode(dim_pin , OUTPUT);
  attachInterrupt(0, light, FALLING);     // прерывание для детектора нуля (2 пин!!!)

  pinMode(brightness_pin_1, INPUT_PULLUP);
  pinMode(brightness_pin_2, INPUT_PULLUP);
  pinMode(brightness_pin_3, INPUT_PULLUP);
  pinMode(brightness_pin_4, INPUT_PULLUP);
  pinMode(brightness_pin_adjustable, INPUT_PULLUP);
}

void light() {
  if (value > 0 && value < 255) {
    delayMicroseconds(33*(255-value));    // вместо 33 былло 34, так между прочим. но если value = 5 лампа припадошно мигает
    digitalWrite(dim_pin , HIGH);
    delayMicroseconds(500);
    digitalWrite(dim_pin , LOW);
  }
}

int get_value_by_percent (int percent) {
  return percent * max_value / 100;
}

void adjusting_mode() {
  if (digitalRead(brightness_pin_1) == LOW) {
    value = min_value;
  } else if (digitalRead(brightness_pin_2) == LOW) {
    value = get_value_by_percent(50);
  } else if (digitalRead(brightness_pin_3) == LOW) {
    value = get_value_by_percent(70);
  } else if (digitalRead(brightness_pin_4) == LOW) {
    value = max_value;
  } else if (digitalRead(brightness_pin_adjustable) == LOW && (millis() - brightness_timing >= 20)) {
    if (should_increase && value < max_value) {
      value += 1;
    } else if (!should_increase && value > min_value) {
      value -= 1;
    } else {
      should_increase = !should_increase;
    }

    brightness_timing = millis();
  }
}

void update_eeprom_value() {
  if (millis() - eeprom_timing >= 1000) {
    EEPROM.put(value_eeprom_addr, value);

    eeprom_timing = millis();
  }
}

void loop() {
  adjusting_mode();
  update_eeprom_value();
}

