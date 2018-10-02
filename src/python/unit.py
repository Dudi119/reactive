import sys
import ast
from os.path import dirname, join
import inspect
from wiring import Edge
from collections import namedtuple, Iterable
import type_checking

sys.path.append(join(dirname(__file__), '../../bin/'))
import _pyNode

class ReturnDescriptor:
    __slots__ = ['name', 'type']
    returnTuple = namedtuple('ReturnTuple', ['name', 'type'])

    @type_checking.runtime_check
    def __init__(self, name : str, type):
        self.name = name
        self.type = type

class FunctionParser:
    returnAstValueParser = {ast.Num : lambda node : ReturnDescriptor.returnTuple('', type(node.n)),
                            ast.Name : lambda node : ReturnDescriptor.returnTuple(node.id, None)}
    @staticmethod
    @type_checking.runtime_check
    def getDescriptor(func : type_checking.Function):
        astTree = ast.parse(inspect.getsource(func))
        if len(astTree.body) == 1 and isinstance(astTree.body[0],ast.FunctionDef):
            funcDefBody = astTree.body[0].body
        else:
            raise TypeError('Function ast tree is required')

        return FunctionParser.getReturnDescriptors(funcDefBody)

    @staticmethod
    def getReturnDescriptors(funcDefBody):
        returnDescriptors = []
        for node in funcDefBody:
            if isinstance(node, ast.Return):
                returnTuple = FunctionParser.returnAstValueParser[type(node.value)](node.value)
                returnDescriptors.append(ReturnDescriptor(returnTuple.name, returnTuple.type))

        return returnDescriptors

class UnitOutDescriptor:
    def __init__(self, id : int, name : str, type):
        self._id = id
        self._name = name
        self._type = type


class UnitDescriptor:
    def __init__(self, func):
        self._func = func
        self._outDescriptors = []
        id = 0
        returnDescriptors = FunctionParser.getDescriptor(self._func)
        for returnDescriptor in returnDescriptors:
           self._outDescriptors.append(UnitOutDescriptor(id, returnDescriptor.name, returnDescriptor.type))

    def __call__(self, *args, **kwargs):
        funcSig = inspect.signature(self._func)
        #unpack outdescriptors
        normalize_args = []
        for value, param in zip(args,funcSig.parameters.values()):
            if type(param.default) == Edge:
                normalize_args.extend(value) if isinstance(value, Iterable) else normalize_args.append(value)
            else:
                normalize_args.append(value)

        sig = _pyNode.PyFunctionSignature()
        for value, param in zip(normalize_args,funcSig.parameters.values()):
            if type(param.default) == Edge:
                sig.addParam(_pyNode.pySignature_ParameterUpdate.Edge, param.default.type, None)
            else:
                sig.addParam(_pyNode.pySignature_ParameterUpdate.Scalar, type(value), value)

        return tuple(self._outDescriptors)

def unit(func):
    return UnitDescriptor(func)