import sys
from os.path import dirname, join
import os

print(os.getpid())

sys.path.append(join(dirname(__file__), '../bin/'))
sys.path.append(join(dirname(__file__), '../src/python/'))
from datetime import timedelta
from unit import unit, parallelUnit
from wiring import Edge, __Output__, const, curve
from graph import sub_graph, graph
from graph_engine import GraphEngine
import _core

edge = Edge[int]

@parallelUnit
def unitA(inputA = Edge[int]):
    __Outputs__(a = str)
    print('unitA - inputEdgeA value = {0}'.format(inputA))
    x = 5
    a = 'hello Dudi'
    return a
    print(x)
    return x

@parallelUnit
def unitB(scalarInt, inputA = Edge[str], inputB = Edge[str]):
    print('unitB - scalarInt value = {0}, inputEdgeA value = {1}, inputEdgeB value = {2}'.format(scalarInt, inputA, inputB))
    return 5

@sub_graph
def sub_graph_a():
    a, x = unitA(const(5, timedelta(seconds=3)))
    unitB(1, a, curve(['A', 'B'], [timedelta(microseconds=100), timedelta(seconds=5)]))
    return x

@graph
def main():
    x = sub_graph_a()

class Logger:
    def __enter__(self):
        _core.Logger.instance().start(_core.Severity.Info)

    def __exit__(self, exc_type, exc_val, exc_tb):
        _core.Logger.instance().terminate()

if __name__ == "__main__":
    with Logger():
        _core.Environment.instance().init()
        GraphEngine().start(main, timedelta(minutes=5))



