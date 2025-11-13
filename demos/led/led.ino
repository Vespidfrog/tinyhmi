// Demonstração 2 - RGB LED

#include <Adafruit_SSD1306.h>
#include "tinyihm1file.h"

#define NUM_COLORS 3

#define PIN_SEL 4 // botão p/ selecionar uma cor
#define PIN_SUB 3 // botãp p/ diminuir o valor da cor selecionada
#define PIN_ADD 2 // botão p/ aumentar o valor da cor selecionada

#define PIN_R 9
#define PIN_G 10
#define PIN_B 11

#define A_OUT_MIN 0   // valor mín. da saída analógica do Arduino
#define A_OUT_MAX 255 // valor máx. da saída analógica do Arduino

int valueR = 0;
int valueG = 0;
int valueB = 0;

Adafruit_SSD1306 display(128, 64, &Wire, 4);

Hslider sliderR(&display, 16, 16, 96, 8, &valueR, A_OUT_MIN, A_OUT_MAX);
Hslider sliderG(&display, 16, 32, 96, 8, &valueG, A_OUT_MIN, A_OUT_MAX);
Hslider sliderB(&display, 16, 48, 96, 8, &valueB, A_OUT_MIN, A_OUT_MAX);

// usa array de ponteiros p/ mudar cor dependendo do índice atual
int* color[] = { &valueR, &valueG, &valueB };

// permite polimorfismo dinâmico
Widget* widget[] = { &sliderR, &sliderG, &sliderB };

int index = 0; // usado pra lógica de input e desenho

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  pinMode(PIN_SEL, INPUT_PULLUP);
  pinMode(PIN_SUB, INPUT_PULLUP);
  pinMode(PIN_ADD, INPUT_PULLUP);
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  display.clearDisplay(); // remove logo da Adafruit

  // desenha todos os widgets por meio do polimorfismo 
  for (int i = 0; i < NUM_COLORS; i++) widget[i]->update();

  display.display(); // envia dados ao display
}

void loop() {
  // atualiza valor da cor selecionada. note que % é p/ comportamento cíclico
  if (digitalRead(PIN_SEL) == LOW) index = (index + 1) % 3; 
  if (digitalRead(PIN_ADD) == LOW) *color[index] += 25;
  if (digitalRead(PIN_SUB) == LOW) *color[index] -= 25;

  // mantém valor da cor selecionada dentro dos limites do analogWrite()
  *color[index] = constrain(*color[index], A_OUT_MIN, A_OUT_MAX);
  widget[index]->update(); // atualiza somente o widget selecionado - mais leve

  analogWrite(PIN_R, valueR);
  analogWrite(PIN_G, valueG);
  analogWrite(PIN_B, valueB);

  display.display(); // envia dados ao display
  delay(50);
}
