#pragma once
#include "State.h"
class MainState :
	public State
{
public:
	MainState();
	~MainState();
	void Start() override;
	void Update() override;
	void End() override;
};

