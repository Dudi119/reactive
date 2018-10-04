import ast
from collections import OrderedDict
from collections import namedtuple
import inspect
import types
from itertools import dropwhile
import type_checking

class ReturnDescriptor:
    __slots__ = ['name', 'type']
    returnTuple = namedtuple('ReturnTuple', ['name', 'type'])

    @type_checking.runtime_check
    def __init__(self, name : str, type):
        self.name = name
        self.type = type
'''
def func(x, y):                     def func(x, y):
    __Outputs__(z=int) ---------->      pass
'''
class FunctionTransformer( ast.NodeTransformer ):
    def visit_Expr(self, node):
        if isinstance(node.value, ast.Call) and node.value.func.id == '__Outputs__':
            return ast.copy_location(ast.Pass(), node)
        else:
            return node

class FunctionParser:
    returnAstValueParser = {ast.Num : lambda node : ReturnDescriptor.returnTuple('', type(node.n)),
                            ast.Name : lambda node : ReturnDescriptor.returnTuple(node.id, None)}

    strToType = {'int' : int,
                 'str' : str}

    @staticmethod
    def getReturnDescriptors(funcDefBody):
        returnDescriptors = OrderedDict()
        for node in funcDefBody:
            if isinstance(node, ast.Return):
                returnTuple = FunctionParser.returnAstValueParser[type(node.value)](node.value)
                if returnTuple.name in returnDescriptors:
                    pass
                else:
                    returnDescriptors[returnTuple.name] = ReturnDescriptor(returnTuple.name, returnTuple.type)
            if isinstance(node, ast.Expr) and isinstance(node.value, ast.Call):
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
    def transformFunction(func : type_checking.Function):
        astTree = ast.parse(inspect.getsource(func), filename=func.__code__.co_filename, mode='exec')

        FunctionTransformer().visit(astTree)
        ast.increment_lineno(astTree, func.__code__.co_firstlineno - 1)
        codeObject = compile(astTree, filename=func.__code__.co_filename, mode='exec')
        return types.FunctionType(codeObject.co_consts[0], func.__globals__, func.__name__, func.__defaults__, func.__closure__)

