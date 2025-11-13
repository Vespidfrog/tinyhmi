#include <Arduino.h>
#include <Adafruit_GFX.h>

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
