int dimpin = 4;      // Выход диммера на симистор

int min_value = 50;
int max_value = 250;
int value = min_value;       // Начальный уровень диммирования от 0 до 255

int brightness_1 = 5;
int brightness_2 = 6;
int brightness_3 = 7;
int brightness_4 = 8;
int brightness_adjustable = 9;

bool should_increase = true;
unsigned long timing;

void setup() {
  pinMode(dimpin , OUTPUT);               // установим выход диммера
  attachInterrupt(0, light, FALLING);     // прерывание для детектора нуля (2 пин!!!)

  pinMode(brightness_1, INPUT_PULLUP);
  pinMode(brightness_2, INPUT_PULLUP);
  pinMode(brightness_3, INPUT_PULLUP);
  pinMode(brightness_4, INPUT_PULLUP);
  pinMode(brightness_adjustable, INPUT_PULLUP);
}

void light() {                        // функция управления яркостью
  if (value > 0 && value < 255) {         // утановим диммирование, если value не5 равен 0 или 255
    delayMicroseconds(33*(255-value));  // вместо 33 былло 34, так между прочим. но если value = 5 лампа припадошно мигает
    digitalWrite(dimpin , HIGH);
    delayMicroseconds(500);
    digitalWrite(dimpin , LOW);
  }
}

int get_value_by_percent (int percent) {
  return percent * max_value / 100;
}

void adjusting_mode() {
  if (digitalRead(brightness_1) == LOW) {
    value = min_value;
  } else if (digitalRead(brightness_2) == LOW) {
    value = get_value_by_percent(50);
  } else if (digitalRead(brightness_3) == LOW) {
    value = get_value_by_percent(70);
  } else if (digitalRead(brightness_4) == LOW) {
    value = max_value;
  } else if (digitalRead(brightness_adjustable) == LOW && (millis() - timing >= 20)) {
    if (should_increase && value < max_value) {
      value += 1;
    } else if (!should_increase && value > min_value) {
      value -= 1;
    } else {
      should_increase = !should_increase;
    }

    timing = millis();
  }
}

void loop() {
  adjusting_mode();
}

