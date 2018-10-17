#include "Event.h"

namespace reactive{

    Event::Event(UnitNode& consumer)
        :m_consumer(consumer)
    {}

    Event::~Event() {}

    template<typename T> TypedEvent<T>::TypedEvent(UnitNode& consumer, T &&data)
        :Event(consumer), m_data(std::move(data))
    {}

    template<typename T> TypedEvent<T>::~TypedEvent() {}
}
