#pragma once

#include <Third_Party/include/sweetPy/Types/DateTime.h>
#include "sweetPy/Types/TimeDelta.h"
#include "sweetPy/Core/Deleter.h"
#include "InputAdapter.h"

namespace reactive{

class ConstNode : public InputAdapter
{
public:
    ConstNode(int id, sweetPy::object_ptr&& value, const sweetPy::TimeDelta& delta);
    virtual ~ConstNode();
    void PostStart() override;
    void PostStop() override;

private:
    sweetPy::object_ptr m_value;
    sweetPy::TimeDelta m_delta;
};

class ConstNodeFactory
{
public:
    static InputAdapter& Create(int id, PyObject* value, const sweetPy::TimeDelta& delta);
};

}