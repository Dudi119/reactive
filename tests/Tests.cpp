#include <iostream>
#include "gtest/gtest.h"
#include "sweetPy/sweetPy.h"
#include "src/cpp/GraphEngine.h"
#include "src/cpp/ConstNode.h"
#include "src/cpp/CurveNode.h"
#include "AsserterNode.h"

static int _argc;
static char **_argv;

namespace reactive_tests{
class ReactiveUnitTestEnvironment : public ::testing::Environment {
    public:
        ReactiveUnitTestEnvironment() {}

        void SetUp() override {
            core::Logger::Instance().Start(core::TraceSeverity::Info);
            Py_Initialize();
        }

        void TearDown() override {
            Py_Finalize();
        }
    };

    TEST(ReactiveTests, ConstNode) {
        PyObject* val = sweetPy::Object<int>::to_python(5);
        sweetPy::ObjectPtr trackedVal(val, &sweetPy::Deleter::Owner);
        sweetPy::TimeDelta timeDelta(1, 0, 0);
        reactive::InputAdapter& constAdapter = reactive::ConstNodeFactory::Create(reactive::InputAdapter::GenerateId(),
            trackedVal.get(), timeDelta);

        std::function<void(int)> assertionFunction = [](int val) {
            static ushort invocation = 0;
            ASSERT_EQ(val, 5);
            ASSERT_EQ(invocation++, 0); //Should only be invoked once
        };
        std::array<std::reference_wrapper<reactive::InputAdapter>, 1> arguments = {constAdapter};
        auto asserterNode = AsserterNodeFactory::Create(assertionFunction, arguments);
        constAdapter.AddConsumer(*asserterNode);

        reactive::GraphEngine::Instance().Start(sweetPy::TimeDelta(3, 0, 0), true);
        reactive::GraphEngine::Instance().Dispose();
    }

    TEST(ReactiveTests, CurveNode) {
        std::vector<int> expectedValues;
        std::vector<sweetPy::ObjectPtr> data;
        std::vector<sweetPy::TimeDelta> delta;
        for (int idx = 0; idx < 5; idx++)
        {
            PyObject* val = sweetPy::Object<int>::to_python(idx);
            expectedValues.emplace_back(idx);
            data.emplace_back(val, &sweetPy::Deleter::Owner);
            delta.emplace_back(idx + 1, 0, 0);
        }
        reactive::InputAdapter& curveAdapter = reactive::CurveNodeFactory::Create(reactive::InputAdapter::GenerateId(), data, delta);

        std::function<void(int)> assertionFunction = [&](int val) {
            static ushort invocation = 0;
            ASSERT_EQ(expectedValues.at(invocation++), val); //Should only be invoked once
        };
        std::array<std::reference_wrapper<reactive::InputAdapter>, 1> arguments = { curveAdapter };
        auto asserterNode = AsserterNodeFactory::Create(assertionFunction, arguments);
        curveAdapter.AddConsumer(*asserterNode);

        reactive::GraphEngine::Instance().Start(sweetPy::TimeDelta(10, 0, 0), true);
        reactive::GraphEngine::Instance().Dispose();
    }
}

int main(int argc, char **argv) {
    _argc = argc;
    _argv = argv;
    ::testing::AddGlobalTestEnvironment(new reactive_tests::ReactiveUnitTestEnvironment());
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}