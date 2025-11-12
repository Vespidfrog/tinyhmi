#include "widget.h"

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

	~Button() override = default;

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
	
	~Play() override = default;
	
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
