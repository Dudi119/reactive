#include "InputAdapter.h"

namespace reactive{

    InputAdapter::~InputAdapter() {}

    void InputAdapter::PreEngineStart(){}
    void InputAdapter::PostEngineStart(){}
    void InputAdapter::PostEngineStep(){}
    void InputAdapter::PostStop(){}

    void InputAdapter::AddConsumer(UnitNode &consumer)
    {
        m_consumers.emplace_back(consumer);
    }
}