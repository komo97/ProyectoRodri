#pragma once
#include "Window.h"
#include <string>
#include <Windows.h>

class TextWindow :
	public Window
{
public:
	TextWindow();
	~TextWindow();
	void draw() override;
	void update() override;
	void SetTextSpeed(const unsigned short& speed);
	void SetText(const std::string& text);
	void SetAutoScroll(const bool& val);
	void CloseOnScrollEnd(const bool& val);
	void SetScrollKey(const Key& val);
	void SetCloseKey(const Key& val);
	void SetUpScrollKey(const Key& val);
	void SetDownScrollKey(const Key& val);
private:
	std::string _string;
	unsigned short _speed, _scrollLine = 0, _maxScrollLines, _maxLineWidth;
	bool	_autoScroll,
			_closeOnScrollEnd;
	Key	_scrollKey,
		_closeKey,
		_upScroll,
		_downScroll;
	INPUT_RECORD _input[32];
	int _currentLetter = 0;
};

