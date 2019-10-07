#pragma once

#include <vector>
#include <utility>
#include "sweetPy/sweetPy.h"
#include "InputAdapter.h"

namespace reactive{
    class CurveNode: public InputAdapter
    {
    public:
       typedef std::pair<sweetPy::ObjectPtr, sweetPy::TimeDelta> CurvePair;
        typedef std::vector<CurvePair> Curve;
        CurveNode(int id, const std::vector<sweetPy::ObjectPtr>& data, const std::vector<sweetPy::TimeDelta>& delta);
        virtual ~CurveNode();
        void PostStart() override;
        void PostStop() override;
    
    private:
        Curve m_curve;
    };
    
    class CurveNodeFactory
    {
    public:
        static InputAdapter& Create(int id, const std::vector<sweetPy::ObjectPtr>& data, const std::vector<sweetPy::TimeDelta>& delta);
    };
}

