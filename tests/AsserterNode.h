#pragma once

#include <functional>
#include <tuple>
#include <array>
#include "sweetPy/sweetPy.h"
#include "core/AsyncTask.h"
#include "InputAdapter.h"
#include "UnitNode.h"

namespace reactive_tests{
    template<class R, class... Tp>
    class AsserterNode : public reactive::UnitNode
    {
    private:
        static const std::size_t Num_Of_Arguments = sizeof...(Tp);
    public:
        typedef std::function<R(Tp...)> Asserter;
        typedef std::unique_ptr<AsserterNode> Pointer;
        typedef std::reference_wrapper<reactive::InputAdapter> Argument_Type;
        typedef std::array<Argument_Type, Num_Of_Arguments> Arguments;
        AsserterNode(const Asserter& asserter, const Arguments& arguments): m_asserter(asserter),
            m_arguments(arguments)
            {
                std::size_t logicalId = 0;
                for(reactive::InputAdapter& inputEdge : m_arguments)
                {
                    m_inEdgesMapping[inputEdge.GetId()] = logicalId++;
                    m_inEdges.emplace_back(inputEdge);
                }
            }

        void Invoke() override
        {
            Invoke_Impl(std::make_index_sequence<Num_Of_Arguments>());
        }

    private:
        template<std::size_t... Is>
        void Invoke_Impl(std::index_sequence<Is...>)
        {
            m_asserter(GetArgument<Tp>(m_arguments[Is])...);
        }

        template<typename _R>
        _R GetArgument(const reactive::InputAdapter& adapter)
        {
            return sweetPy::Object<_R>::from_python(adapter.GetLastData().get());
        }
    
    private:
        Asserter m_asserter;
        Arguments m_arguments;
    };

    class AsserterNodeFactory
    {
    public:
        template<class R, class... Tp>
        static inline typename AsserterNode<R, Tp...>::Pointer 
            Create(const std::function<R(Tp...)>& assertionFuncion, 
                const std::array<std::reference_wrapper<reactive::InputAdapter>, sizeof...(Tp)>& arguments)
        {
            return std::make_unique<AsserterNode<R, Tp...>>(assertionFuncion, arguments);
        }
    };
}