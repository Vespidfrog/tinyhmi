#include "widget.h"

// usada pra todo widget com mais de 2 valores
class Analog : public Widget {
protected:
	// ponteiro p/ uma var. externa p/ ocupar menos memória
	const int* m_value;
	int m_lastValue;
	int m_minimum;
	int m_maximum;

public:
	Analog(Adafruit_GFX* display, int x, int y, int w, int h, int* value, int minimum, int maximum) :
		Widget(display, x, y, w, h),
		m_value(value),
		m_minimum(minimum),
		m_maximum(maximum),
		// último valor diferente do valor atual p/ forçar desenho do 1º frame
		m_lastValue(minimum - 1) {}
	
	// ainda não vamos fazer override do destrutor nem do update!!
};

// slider vertical
class Vslider : public Analog {
public:
	Vslider(Adafruit_GFX* display, int x, int y, int w, int h, int* value, int minimum, int maximum) :
		Analog(display, x, y, w, h, value, minimum, maximum) {}
	
	~Vslider() override = default;
	
	void update() override {
		if (*m_value == m_lastValue || !m_visible) return;
		
		clearFrame(); // apaga o último frame
		
		// posição do slider
		int x0 = m_x;
		int y0 = m_y;
		
		// calcula a posição do indicador baseado no valor lido
		int yPos = map(*m_value, m_minimum, m_maximum, m_y + m_h, m_y); // de baixo pra cima

    
		// desenha o track do slider
		m_display->drawLine(x0 + m_w / 2, m_y, x0 + m_w / 2, m_y + m_h, WHITE);
		
		// desenha o indicador
		m_display->fillRect(x0, yPos - 2, m_w, 4, WHITE);
		
		m_lastValue = *m_value;
	}
};

// slider horizontal
class Hslider : public Analog {
public:
	Hslider(Adafruit_GFX* display, int x, int y, int w, int h, int* value, int minimum, int maximum) :
		Analog(display, x, y, w, h, value, minimum, maximum) {}
	
	~Hslider() override = default;
	
	void update() override {
		if (*m_value == m_lastValue || !m_visible) return;
		
		clearFrame(); // apaga o último frame
		
		// posição do slider
		int x0 = m_x;
		int y0 = m_y;
		
		// calcula a posição do indicador baseado no valor lido
		int xPos = map(*m_value, m_minimum, m_maximum, m_x, m_x + m_w); // da esquerda pra direita
		xPos = constrain(xPos, m_x + 2, m_x + m_w - 2);

		// desenha o track do slider
		m_display->drawLine(m_x, y0 + m_h / 2, m_x + m_w, y0 + m_h / 2, WHITE);
		
		// desenha o indicador
		m_display->fillRect(xPos - 2, y0, 4, m_h, WHITE);
		
		m_lastValue = *m_value;
	}
};

// potenciômetro / knob
class Knob : public Analog {
public:
	Knob(Adafruit_GFX* display, int x, int y, int w, int h, int* value, int minimum, int maximum) :
		Analog(display, x, y, w, h, value, minimum, maximum) {}
	
	~Knob() override = default;
	
	void update() override {
		// não desenha novo frame se não precisa
		if (*m_value == m_lastValue || !m_visible) return;
	    
	    clearFrame(); // apaga o último frame

		// calcula posição do centro do widget
		int x0 = m_x + m_w / 2;
		int y0 = m_y + m_h / 2;
		int r = min(m_w, m_h) / 2 - 1;

		// desenha a coroa do knob
		m_display->drawCircle(x0, y0, r, WHITE);

		// calcula o ângulo baseado no valor lido
		float angleDeg = map(*m_value, m_minimum, m_maximum, -225, 45);
		float angleRad = (float)angleDeg * radians(1);

		// calcula coordenadas do risquinho do potenciômetro
		int x1 = x0 + (r - m_pad) * cos(angleRad);
		int y1 = y0 + (r - m_pad) * sin(angleRad);

		// desenha o risquinho
		m_display->drawLine(x0, y0, x1, y1, WHITE);	
		
		m_lastValue = *m_value;
	}
};

