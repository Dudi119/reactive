import sys
import ast
from os.path import dirname, join
import inspect
from wiring import Edge
import type_checking

sys.path.append(join(dirname(__file__), '../../bin/'))
import _pyNode

class FunctionParser:
    @staticmethod
    @type_checking.runtime_check
    def getDescriptor(func : type_checking.Function):
        astTree = ast.parse(inspect.getsource(func))
        if len(astTree.body) == 1 and isinstance(astTree.body[0],ast.FunctionDef):
            funcDefBody = astTree.body[0].body
        else:
            raise TypeError('Invalid function ast tree')

class UnitDescriptor:
    def __init__(self, func):
        self._func = func
        FunctionParser.getDescriptor(self._func)

    def __call__(self, *args, **kwargs):
        funcSig = inspect.signature(self._func)
        sig = _pyNode.PyFunctionSignature()
        for value, param in zip(args,funcSig.parameters.values()):
            if type(param.default) == Edge:
                sig.addParam(_pyNode.pySignature_ParameterUpdate.Edge, param.default.type, None)
            else:
                sig.addParam(_pyNode.pySignature_ParameterUpdate.Scalar, type(value), value)

def unit(func):
    return UnitDescriptor(func)