#include "Engine.h"
#include <Windows.h>
#include <iostream>
#include "HelperFunctions.h"
#include "StateManager.h"
#include "MainState.h"
#include "SimplePool.h"

Engine* Engine::_singleton = nullptr;
Engine::Engine()
{
	_closing = false;
}


Engine::~Engine()
{
}

Engine * Engine::getInstance()
{
	return _singleton ? _singleton : _singleton = new Engine();
}

void Engine::app(const size_t & x, const size_t & y, const char* title, const bool& fullScreen)
{
	_width = x;
	_height = y;
	HANDLE hnd = getConsoleHandle();
	CONSOLE_SCREEN_BUFFER_INFOEX info;
	COORD screenCoords;
	memset(&info, 0, sizeof(info));
	GetConsoleScreenBufferInfoEx(hnd, &info);
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(hnd, &cursor);
	cursor.bVisible = FALSE;
	SetConsoleCursorInfo(hnd, &cursor);
	if(info.bFullscreenSupported && fullScreen)
	{
		SetConsoleDisplayMode(hnd, CONSOLE_FULLSCREEN_MODE, &screenCoords);
		_width = screenCoords.X;
		_height = screenCoords.Y;
	}
	else
	{
		screenCoords.X = (SHORT)_width;
		screenCoords.Y = (SHORT)_height;
		std::cout << screenCoords.X << " " << screenCoords.Y << std::endl;
		SMALL_RECT rect = { 0, 0, 10, 10 }; 
		SetConsoleWindowInfo(hnd, TRUE, &rect);  //Primero hacer la ventana peque�a.
		COORD minConsoleSize = { (SHORT)GetSystemMetrics(SM_CXMIN), (SHORT)GetSystemMetrics(SM_CYMIN) };
		if (minConsoleSize.X > screenCoords.X)
			screenCoords.X = _width = minConsoleSize.X;
		if (minConsoleSize.Y > screenCoords.Y)
			screenCoords.Y = _height = minConsoleSize.Y; //Ajustar los tama�os minimos del buffer a lo que el sistema pide.
		std::cout << GetSystemMetrics(SM_CXMIN) << " " << GetSystemMetrics(SM_CYMIN) << std::endl;
		SetConsoleScreenBufferSize(hnd, screenCoords); //Setear el buffer.
		CONSOLE_SCREEN_BUFFER_INFO infof;
		GetConsoleScreenBufferInfo(hnd, &infof); //Conseguir el tama�o correcto de la ventana.
		rect.Right = infof.dwMaximumWindowSize.X - 1;
		rect.Bottom = infof.dwMaximumWindowSize.Y - 1;
		SetConsoleWindowInfo(hnd,TRUE,&rect); //Setear el tama�o de la ventana.
	}
	SetConsoleTitle(title);
	SetConsoleMode(hnd,ENABLE_EXTENDED_FLAGS);
	StateManager::ChangeState(new MainState());
	while (!_closing)
	{
		system("cls");
		StateManager::Update();
		SimplePool::drawAll();
		if (_windows[0])
			_windows[0]->draw();
		if (_windows[1])
			_windows[1]->draw();
		for (int i = 0; i < 2; ++i)
			if (_windows[i])
				_windows[i]->update();
		SetConsoleTextAttribute(hnd, BackgroundColor::BBLACK | LetterColor::WHITE);
		Sleep(100);
	}
}

size_t Engine::getWidth() const
{
	return _width;
}

size_t Engine::getHeight() const
{
	return _height;
}

void Engine::setTextWindowActive(const bool & value)
{
	_windows[0]->SetActive(value);
}

void Engine::setMenuWindowActive(const bool & value)
{
	_windows[1]->SetActive(value);
}

void Engine::setTextWindow(TextWindow* const& val)
{
	if (_windows[0])
		delete _windows[0];
	_windows[0] = val;
}

void Engine::setMenuWindow(MenuWindow* const& val)
{
	if (_windows[1])
		delete _windows[1];
	_windows[1] = val;
}

TextWindow * Engine::getTextWindow() const
{
	return (TextWindow*)_windows[0];
}

MenuWindow * Engine::getMenuWindow() const
{
	return (MenuWindow*)_windows[1];
}

void Engine::AddToBuffer(const PixelArr & data)
{
	COORD pos;
	HANDLE console = getConsoleHandle();
	DWORD nonImportant;
	unsigned short col;
	//if (data.colorSize == PixelArr::SingleColor)
	//	SetConsoleTextAttribute(getConsoleHandle(), data.colors[0]);
	if (data.colorSize == PixelArr::SingleColor)
		col = data.colors[0];
	for (int i = 0; i < data.charSizeY; ++i)
	{
		for (int j = 0; j < data.charSizeX; ++j)
		{
			pos.X = data.posX + j;
			pos.Y = data.posY + i;
			_buffer.characters[getAccessor(pos.X, pos.Y, _buffer.charSizeX)] = data.characters[getAccessor(j, i, data.charSizeX)];
			if (data.colorSize != PixelArr::SingleColor)
				col = data.colors[getAccessor(j, i, data.charSizeX)];
			_buffer.colors[getAccessor(pos.X, pos.Y, _buffer.charSizeX)] = col;
			//if (data.colorSize == PixelArr::MultiColor)
			//	SetConsoleTextAttribute(console, data.colors[getAccessor(j, i, data.charSizeX)]);
			//WriteConsole(console, &data.characters[getAccessor(j, i, data.charSizeX)], 1, &nonImportant, NULL);
		}
	}
}

void Engine::CloseApp()
{
	_closing = true;
	if (_windows[1])
		delete _windows[1];
	if (_windows[0])
		delete _windows[0];
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(getConsoleHandle(), &cursor);
	cursor.bVisible = TRUE;
	SetConsoleCursorInfo(getConsoleHandle(), &cursor);
}

void Engine::draw()
{
}
