// Demonstração 1 - MP3 Player

#include <Adafruit_SSD1306.h>
#include "tinyihm1file.h"

int comprimento = 100; // comprimento da música
int tempo = 0;         // tempo atual do playhead
bool tocando = false;  // estado de reprodução

Adafruit_SSD1306 tela(128, 64, &Wire, 4);
#define PINO_BOTAO 2

Hslider playhead(&tela, 16, 48, 92, 16, &tempo, 0, comprimento);
Play playpause(&tela, 50, 16, 24, 24, &tocando);

Widget* widget[] = { &playhead, &playpause };
#define NUM_WIDGETS 2

// setpu com pinMode, configuração de tela, etc. nada demais...
void setup() {
  pinMode(PINO_BOTAO, INPUT_PULLUP);
  tela.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  tela.clearDisplay();
  tela.display();
}

void loop() {
  // se botão for pressionado, mudar estado do player
  if (digitalRead(PINO_BOTAO) == LOW) tocando = !tocando;

  // polimorfismo dinâmico!!
  for (int i = 0; i < NUM_WIDGETS; i++) widget[i]->update();

  if (tocando && tempo >= comprimento) {
    tocando = false;
    tempo = 0;
  }

  else if (tocando && tempo < comprimento) tempo = tempo + 2;
    
  tela.display();
}
