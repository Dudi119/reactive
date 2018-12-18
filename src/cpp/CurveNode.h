#pragma once

#include <vector>
#include <utility>
#include <Third_Party/include/sweetPy/Types/DateTime.h>
#include "sweetPy/Types/TimeDelta.h"
#include "sweetPy/Core/Deleter.h"
#include "InputAdapter.h"

namespace reactive{
    class CurveNode: public InputAdapter
    {
    public:
       typedef std::pair<sweetPy::object_ptr, sweetPy::TimeDelta> CurvePair;
        typedef std::vector<CurvePair> Curve;
        CurveNode(int id, const std::vector<sweetPy::object_ptr>& data, const std::vector<sweetPy::TimeDelta>& delta);
        virtual ~CurveNode();
        void PostStart() override;
        void PostStop() override;
    
    private:
        Curve m_curve;
    };
    
    class CurveNodeFactory
    {
    public:
        static InputAdapter& Create(int id, const std::vector<sweetPy::object_ptr>& data, const std::vector<sweetPy::TimeDelta>& delta);
    };
}

