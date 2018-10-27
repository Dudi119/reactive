#include "Event.h"

namespace reactive{

    Event::Event(GraphNode& consumer)
        :m_consumer(consumer)
    {}

    Event::~Event() {}
}
