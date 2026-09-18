#pragma once
#include <memory>

class State;

class StateMachine
{
    public:
        void Update(float dt);
        void ChangeState(std::unique_ptr<State> new_state);

    private:
        std::unique_ptr<State> current_state;
};