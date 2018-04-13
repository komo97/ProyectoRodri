#pragma once
class State
{
public:
	State();
	virtual ~State();
	virtual void Start();
	virtual void Update();
	virtual void End();
};

