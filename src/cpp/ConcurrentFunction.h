#pragma once

#include <memory>
#include <string>
#include <atomic>
#include "core/Exception.h"

namespace reactive{
    
    template<std::size_t Size>
    class ConcurrentFunction
    {
    public:
        typedef ConcurrentFunction<Size> Self;
        typedef std::unique_ptr<Self> Pointer;

        ConcurrentFunction() :m_id(GenerateId()) {}//Should be private due to partial state initialization (Factory should be the only place to initialize an instance), but will require to know any one who is dependet on default constructor otherwise and to friend it.

        ConcurrentFunction(ConcurrentFunction& other)
        : m_function(other.m_function)
        {
        }

        ConcurrentFunction(ConcurrentFunction&& other)
        : m_id(other.m_id), m_function(std::move(other.m_function))
        {
            other.Clear();
        }

        Self& operator=(ConcurrentFunction& rhs)
        {
            m_function = rhs.m_function;
            return *this;
        }

        Self& operator=(ConcurrentFunction&& rhs)
        {
            m_function = std::move(rhs.m_function);
            m_id = rhs.m_id;
            rhs.Clear();
            return *this;
        }

        void SetFunction(const std::string& function)
        {
            std::size_t actualSize = sizeof(Self) + function.length();
            if(actualSize > Size)
            {
                throw core::Exception(__CORE_SOURCE, "ConcurrentFunction size %d exceeds allowed size - %d", actualSize, Size);
            }
            m_function = function;
        }

        const std::string& GetFunction() const
        {
            return m_function;
        }

    private:
        static int GenerateId()
        {
            static std::atomic_int id(0);
            return id++;
        }

        void Clear()
        {
            m_function.clear();
            m_id = -1;
        }

    private:
        int m_id;
        std::string m_function;
    };

    template<std::size_t Size>
    typename ConcurrentFunction<Size>::Pointer MakeConcurrentFunction(const std::string& function)
    {
        typename ConcurrentFunction<Size>::Pointer concurrentFunction(new ConcurrentFunction<Size>());
        concurrentFunction->SetFunction(function);
        return concurrentFunction;
    }
}