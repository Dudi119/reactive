import inspect
from graph_engine import GraphEngine
import type_checking

class SubGraphDescriptor:
    def __init__(self, func):
        self._func = func

    def __call__(self, *args, **kwargs):
        #unpack outdescriptors
        normalize_args = []
        for value in args:
            if isinstance(value, UnitOutDescriptorsTuple):
                normalize_args.extend(value)
            else:
                normalize_args.append(value)

        for key, value in kwargs.items():
            if isinstance(value, UnitOutDescriptorsTuple):
                raise ValueError('Multiple edges were provided to a single keyword argument - {0}'.format(key))

        return self._func(*normalize_args, **kwargs)


@type_checking.runtime_check
def sub_graph(func : type_checking.Function):
    return SubGraphDescriptor(func)

@type_checking.runtime_check
def graph(func : type_checking.Function):

    funcSig = inspect.signature(func)
    if len(funcSig.parameters) > 0:
        raise TypeError('graph cannot have any parameters')

    def _graph():
        GraphEngine().clear()
        func()
        GraphEngine().build()

    return _graph
