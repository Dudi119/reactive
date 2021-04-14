import ast
from collections import OrderedDict
from collections import namedtuple
import inspect
import types
import type_checking


import sys
from os.path import dirname, join
sys.path.append(join(dirname(__file__), '../../bin/'))
from _pyNode import UnitNode

class ReturnDescriptor:
    __slots__ = ['name', 'type']
    returnTuple = namedtuple('ReturnTuple', ['name', 'type', 'value'])

    @type_checking.runtime_check
    def __init__(self, name : str, type):
        self.name = name
        self.type = type

returnAstValueParser = {ast.Num : lambda node : ReturnDescriptor.returnTuple('unknown_int', type(node.n), node.n), #Not more than one anonymus int out edge per unit
                        ast.Str : lambda node : ReturnDescriptor.returnTuple('unknown_str' + node.s, type(node.s), node.s), #Not more than one anonymus str out edge per unit
                        ast.Name : lambda node : ReturnDescriptor.returnTuple(node.id, None, node.id),
                        ast.Constant : lambda node : ReturnDescriptor.returnTuple('unknown_int' if type(node.n) == int else 'unknown_str', type(node.n), node.n)} #Not more than one anonymus int out edge per unit
'''
def func(x, y):                     def func(x, y):
    __Outputs__(z=int) ---------->      pass
    return val         ---------->      _pyNode.UnitNode.produceOutEdgeData(node, id, value)
'''
class FunctionTransformer( ast.NodeTransformer ):
    returnAstNameParser = {ast.Num : lambda node : 'unknown_int', #Not more than one anonymus int out edge per unit
                         ast.Str : lambda node : 'unknown_str', #Not more than one anonymus str out edge per unit
                         ast.Name : lambda node : node.id}
    def __init__(self, unitDescriptor):
        self._unitDescriptor = unitDescriptor

        super(FunctionTransformer, self).__init__()
    def visit_Expr(self, node):
        if isinstance(node.value, ast.Call) and isinstance(node.value.func, ast.Name) and node.value.func.id == '__Outputs__':
            return ast.copy_location(ast.Pass(), node)
        else:
            return node

    def visit_Return(self, node):
        edgeId, returnTuple = next(((outDescriptor.id, returnAstValueParser[type(node.value)](node.value)) for outDescriptor in self._unitDescriptor.outDescriptors if outDescriptor._name == returnAstValueParser[type(node.value)](node.value).name), None)
        funcArg = ast.Name('nativeFunc' + str(id(self._unitDescriptor.nativeNode)), ast.Load())
        nodeArg = ast.Name('nativeNode' + str(id(self._unitDescriptor.nativeNode)), ast.Load())
        idArg = ast.Num(edgeId)
        valueArg = None
        if 'unknown_int' == returnTuple.name:
           valueArg = ast.Num(returnTuple.value)
        elif 'unknown_str' == returnTuple.name:
            valueArg = ast.Str(returnTuple.value)
        else:
            valueArg = ast.Name(returnTuple.value, ast.Load())

        return ast.copy_location(ast.Return(ast.Call(funcArg, [nodeArg, idArg, valueArg], [])), node)

class FunctionParser:
    strToType = {'int' : int,
                 'str' : str}

    @staticmethod
    def getReturnDescriptors(funcDefBody):
        returnDescriptors = OrderedDict()
        for node in funcDefBody:
            if isinstance(node, ast.Return):
                returnTuple = returnAstValueParser[type(node.value)](node.value)
                if returnTuple.name in returnDescriptors:
                    pass
                else:
                    returnDescriptors[returnTuple.name] = ReturnDescriptor(returnTuple.name, returnTuple.type)
            if isinstance(node, ast.Expr) and isinstance(node.value, ast.Call) and isinstance(node.value.func, ast.Name):
                exprFunc = node.value.func
                if exprFunc.id == '__Outputs__':
                    for arg in node.value.keywords:
                        returnDescriptors[arg.arg] = ReturnDescriptor(arg.arg, FunctionParser.strToType[arg.value.id])

        return returnDescriptors

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
    @type_checking.runtime_check
    def transformFunction(func : type_checking.Function, unitDescriptor):
        if 'nativeFunc' + str(id(unitDescriptor.nativeNode)) in func.__globals__ or 'nativeNode' + str(id(unitDescriptor.nativeNode))  in func.__globals__ :
            raise ValueError('virtual address is already being used, no duplications allowed')
        func.__globals__['nativeFunc' + str(id(unitDescriptor.nativeNode))] = UnitNode.produceOutEdgeData
        func.__globals__['nativeNode' + str(id(unitDescriptor.nativeNode))] = unitDescriptor.nativeNode
        astTree = ast.parse(inspect.getsource(func), filename=func.__code__.co_filename, mode='exec')

        FunctionTransformer(unitDescriptor).visit(astTree)
        ast.fix_missing_locations(astTree)
        ast.increment_lineno(astTree, func.__code__.co_firstlineno - 1)
        codeObject = compile(astTree, filename=func.__code__.co_filename, mode='exec')
        return types.FunctionType(codeObject.co_consts[0], func.__globals__, func.__name__, func.__defaults__, func.__closure__)

