#pragma once

#include "sweetPy/sweetPy.h"
#include "InputAdapter.h"

namespace reactive{

class ConstNode : public InputAdapter
{
public:
    ConstNode(int id, sweetPy::ObjectPtr&& value, const sweetPy::TimeDelta& delta);
    virtual ~ConstNode();
    void PostStart() override;
    void PostStop() override;

private:
    sweetPy::ObjectPtr m_value;
    sweetPy::TimeDelta m_delta;
};

class ConstNodeFactory
{
public:
    static InputAdapter& Create(int id, PyObject* value, const sweetPy::TimeDelta& delta);
};

}