#include "ConstNode.h"

namespace reactive{
    ConstNode::ConstNode(sweetPy::object_ptr &&value, const sweetPy::TimeDelta &delta)
        : m_value(std::move(value)), m_delta(delta)
    {}

    ConstNode::~ConstNode() {}
}
