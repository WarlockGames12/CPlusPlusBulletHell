#include "statemachine.hpp"
#include "state.hpp"

void StateMachine::Update(float dt)
{
    if (current_state)
        current_state->Update(dt);
}

void StateMachine::ChangeState(std::unique_ptr<State> new_state)
{
    if(current_state)
        current_state->Exit();
    current_state = std::move(new_state);
    if (current_state)
        current_state->Enter();
}