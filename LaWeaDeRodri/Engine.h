#pragma once
#include "TextWindow.h"
#include "MenuWindow.h"
#include "Window.h"

typedef struct PixelArr {
	char* characters;
	unsigned short* colors;
	size_t charSizeX, charSizeY, posX, posY;
	enum {
		SingleColor,
		MultiColor,
	} colorSize;
}PixelArr;

class Engine
{
public:
	Engine();
	~Engine();
	static Engine* getInstance();
	void app(const size_t& x, const size_t& y, const char* title, const bool& fullScreen);
	size_t getWidth() const;
	size_t getHeight() const;
	void setTextWindowActive(const bool& value);
	void setMenuWindowActive(const bool& value);
	void setTextWindow(TextWindow* const& val);
	void setMenuWindow(MenuWindow* const& val);
	TextWindow* getTextWindow() const;
	MenuWindow* getMenuWindow() const;
	void AddToBuffer(const PixelArr& data);
	void CloseApp();
private:
	static Engine* _singleton;
	size_t _width, _height;
	bool _closing;
	Window* _windows[2] = { nullptr, nullptr };
	PixelArr _buffer, _backBuffer;
	void draw();
};

