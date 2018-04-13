#pragma once
#include <stack>
#include "State.h"
class StateManager
{
public:
	StateManager();
	~StateManager();
	static void ChangeState(State* const& state);
	static void EndState();
	static void Update();
private:
	static std::stack<State*> _stateStack;
};

