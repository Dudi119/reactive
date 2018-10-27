#pragma once

#include "sweetPy/Types/TimeDelta.h"
#include "sweetPy/Core/Deleter.h"
#include "InputAdapter.h"

namespace reactive{

class ConstNode : public InputAdapter
{
public:
    ConstNode(sweetPy::object_ptr&& value, const sweetPy::TimeDelta& delta);
    virtual ~ConstNode();
    void PostStart() override;

private:
    sweetPy::object_ptr m_value;
    sweetPy::TimeDelta m_delta;
};

}