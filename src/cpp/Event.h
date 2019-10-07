#pragma once

#include <memory>
#include <chrono>
#include <type_traits>
#include <Python.h>
#include "sweetPy/sweetPy.h"
#include "core/Exception.h"

namespace reactive
{
    class GraphNode;

    class Event
    {
    public:
        typedef std::chrono::system_clock::time_point TimePoint;

        Event(GraphNode& consumer);
        virtual ~Event();
        GraphNode& GetConsumer() { return m_consumer; }
        virtual sweetPy::ObjectPtr GetData();

    private:
        GraphNode& m_consumer;
    };

    template<typename T>
    class TypedEvent : public Event
    {
    public:
        TypedEvent(GraphNode& consumer, const T& data)
                :Event(consumer), m_data(data) {}
        TypedEvent(GraphNode& consumer, const sweetPy::ObjectPtr& data)
                :Event(consumer), m_data(sweetPy::Object<T>::from_python(data.get())) {}

        virtual ~TypedEvent(){}
        T& GetData() const { return m_data; }
        sweetPy::ObjectPtr GetData() override
        {
            return sweetPy::ObjectPtr(sweetPy::Object<T>::to_python(m_data), &sweetPy::Deleter::Owner);
        }

    private:
        T m_data;
    };

    class TypedEventFactory
    {
    public:
        template<typename... Arguments>
        static std::unique_ptr<Event> Create(PyTypeObject* type, Arguments&&... arguments)
        {
            if(type == &PyLong_Type)
                return std::unique_ptr<Event>(new TypedEvent<int>(std::forward<Arguments>(arguments)...));
            else if(type == &PyFloat_Type)
                return std::unique_ptr<Event>(new TypedEvent<double>(std::forward<Arguments>(arguments)...));
            else if(type == &PyUnicode_Type)
                return std::unique_ptr<Event>(new TypedEvent<sweetPy::AsciiString>(std::forward<Arguments>(arguments)...));
            else if(type == &PyBytes_Type)
                return std::unique_ptr<Event>(new TypedEvent<std::string>(std::forward<Arguments>(arguments)...));
            else if(type == &PyBool_Type)
                return std::unique_ptr<Event>(new TypedEvent<bool>(std::forward<Arguments>(arguments)...));
            else
                throw core::Exception(__CORE_SOURCE, "PyTypeObject is not supported.");
        }
    };
}