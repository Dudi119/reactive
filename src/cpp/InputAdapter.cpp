#include "InputAdapter.h"

namespace reactive{

    InputAdapter::~InputAdapter() {}

    void InputAdapter::PreStart(){}
    void InputAdapter::PostStart(){}
    void InputAdapter::PostStep(){}
    void InputAdapter::PostStop(){}

    void InputAdapter::AddConsumer(UnitNode &consumer)
    {
        m_consumers.emplace_back(consumer);
    }
}