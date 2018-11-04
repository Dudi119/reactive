#include "Event.h"

namespace reactive{

    Event::Event(GraphNode& consumer)
        :m_consumer(consumer)
    {}

    Event::~Event() {}

    sweetPy::object_ptr Event::GetData()
    {
        throw core::Exception(__CORE_SOURCE, "Functionality is not supported.");
    }
}
