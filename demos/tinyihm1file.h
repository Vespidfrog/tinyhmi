#ifndef TINYIHM1FILE_H
#define TINYIHM1FILE_H

class Widget {
protected:
	const int m_pad = 2;

	// ponteiro pra um display suportado pela GFX
	Adafruit_GFX* m_display;
	
	int m_x = 0, m_y = 0;
	int m_w = 0, m_h = 0;
	bool m_visible = true;

	// limpa o frame anterior retângulo da cor do fundo num oled (preto)
	void clearFrame() { 
    m_display->fillRect(m_x - 5, m_y - 5, m_w + 5, m_h + 5, BLACK); 
  }

public:
	// construtor normal
	Widget(Adafruit_GFX* display, int x, int y, int w, int h) :
		m_display(display), m_x(x), m_y(y), m_w(w), m_h(h) {}
	
	// destrutor virtual pois é classe abstrata
	virtual ~Widget() = default;

	// setters p/ cada atributo
	void setX(int px) { m_x = px; }
	void setY(int px) { m_y = px; }
	void setWidth(int px) { m_w = px; }
	void setHeight(int px) { m_h = px; }
	void setVisibility(bool visible) { 
		m_visible = visible;	
		if (!visible) clearFrame();
	}

	// getters p/ cada atributo
	int getX() const { return m_x; }
	int getY() const { return m_y; }
	int getWidth() const { return m_w; }
	int getHeight() const { return m_h; }
	bool isVisible() const { return m_visible; }
	
	// o desenho do widget é específico pra cada widget
	// precisamos do polimorfismo, com uma função virtual
	virtual void update() = 0;
};




// usada pra todo widget com 2 valores
class Digital : public Widget {
protected:
	// ponteiro p/ uma var. externa p/ ocupar menos memória
	const bool* m_state;
	
	bool m_lastState;
	
public:
	Digital(Adafruit_GFX* display, int x, int y, int w, int h, bool* state) :
		Widget(display, x, y, w, h),
		m_state(state),
		// último estado diferente do estado atual pra forçar desenho do 1º frame
		m_lastState(!*state) {}
		
	// ainda não vamos fazer override do destrutor nem do update!!
};

// botão simples
class Button : public Digital {
public:
	Button(Adafruit_GFX* display, int x, int y, int w, int h, bool* state) :
		Digital(display, x, y, w, h, state) {}

	~Button() = default;

	void update() override {
		// não redesenha se o estado não mudou
		if (*m_state == m_lastState || !m_visible) return;

		clearFrame(); // apaga o último frame

		// desenha o botão ligado/desligado
		if (*m_state)
			m_display->fillRect(m_x, m_y, m_w, m_h, WHITE);
		else 
			m_display->drawRect(m_x, m_y, m_w, m_h, WHITE);

		// atualiza o último estado
		m_lastState = *m_state;
	}
};


// botão play / pause
class Play : public Digital {	
public:
	Play(Adafruit_GFX* display, int x, int y, int w, int h, bool* state) :
		Digital(display, x, y, w, h, state) {}
	
	~Play() = default;
	
	void update() override {
		// não desenha novo frame se não precisa
		if (*m_state == m_lastState || !m_visible) return;
		
		clearFrame(); // apaga o último frame
		
		// se tiver tocando, mostrar ícone de pause
		if (*m_state) {
			int barWidth = m_w / 6;	// comprimento de cada barra
			
			m_display->fillRect(m_x + m_w * 0.25f, m_y + m_h * 0.2f, barWidth, m_h * 0.6f, WHITE);
			m_display->fillRect(m_x + m_w * 0.55f, m_y + m_h * 0.2f, barWidth, m_h * 0.6f, WHITE);
		} else {
			// se tiver pausado, mostrar ícone de play 
			m_display->fillTriangle(
				m_x + m_w * 0.3f, m_y + m_h * 0.2f,
				m_x + m_w * 0.3f, m_y + m_h * 0.8f,
				m_x + m_w * 0.8f, m_y + m_h * 0.5f,
				WHITE
			);
		}
		
		// atualiza estados
		m_lastState = *m_state;
	}
};




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
	
	~Vslider() = default;
	
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
	
	~Hslider() = default;
	
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
	
	~Knob() = default;
	
	void update() override {
		// não desenha novo frame se não precisa
		if (*m_value == m_lastValue || !m_visible) return;
	    
	    clearFrame(); // apaga o último frame

		// calcula posição do centro do widget
		int x0 = m_x + m_w / 2;
		int y0 = m_y + m_h / 2;
		int r = min(m_x, m_y) / 2 - 1;

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

#endif // TINYIHM1FILE_H
