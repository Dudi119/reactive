import sys
import cloudpickle
from os.path import dirname, join
import inspect
from unit_parser import FunctionParser
import collections

sys.path.append(join(dirname(__file__), '../../bin/'))
import _pyNode

class UnitDescriptor:
    def __init__(self, func, funcAsByteArray, isConcurrent):
        self._outDescriptors = []
        self._funcAsByteArray = funcAsByteArray
        self._isConcurrent = isConcurrent
        self.inScore = 0
        returnDescriptors = FunctionParser.getDescriptor(func)
        for returnDescriptor in returnDescriptors.values():
           self._outDescriptors.append(OutDescriptor(idGenerator(), returnDescriptor.name, returnDescriptor.type, self))

        self._func = func

    def __call__(self, *args, **kwargs):
        GraphEngine().addNode(self)

        funcSig = inspect.signature(self._func)
        #unpack outdescriptors
        normalize_args = []
        for value in args:
            if isinstance(value, OutDescriptorsTuple):
                normalize_args.extend(value)
            else:
                normalize_args.append(value)

        for key, value in kwargs.items():
            if isinstance(value, OutDescriptorsTuple):
                raise ValueError('Multiple edges were provided to a single keyword argument - {0}'.format(key))

        self._sig = _pyNode.PyFunctionSignature()
        for value, param in zip(normalize_args,funcSig.parameters.values()):
            if type(param.default) == Edge:
                if not isinstance(value, OutDescriptor):
                    raise ValueError('A non edge was connected to an edge, edge name - {0}'.format(param.name))
                value.addConsumer(self)
                value.type = param.default.type

                if value.node: #if current outDescriptor is owned by a node
                    inputAdapter = value.node.nativeNode.getOutEdge(value.id)
                else: #current outDescriptor represent the inputAdapter
                    inputAdapter = value.nativeNode
                self._sig.addParam(_pyNode.pySignature_ParameterUpdate.Edge, value.type, inputAdapter)
            else:
                if isinstance(value, OutDescriptor):
                    raise ValueError('An edge was connected to a non edge type, param name - {0}'.format(param.name))
                self._sig.addParam(_pyNode.pySignature_ParameterUpdate.Scalar, type(value), value)

        for argName, value in kwargs.items():
            param = funcSig.parameters[argName]
            if type(param.default) == Edge:
                if not isinstance(value, OutDescriptor):
                    raise ValueError('A non edge was connected to an edge, edge name - {0}'.format(param.name))
                value.addConsumer(self)
                value.type = param.default.type

                if value.node: #if current outDescriptor is owned by a node
                    inputAdapter = value.node.nativeNode.getOutEdge(value.id)
                else: #current outDescriptor represent the inputAdapter
                    inputAdapter = value.nativeNode

                self._sig.addParam(_pyNode.pySignature_ParameterUpdate.Edge, value.type, inputAdapter)
            else:
                if isinstance(value, OutDescriptor):
                    raise ValueError('An edge was connected to a non edge type, param name - {0}'.format(param.name))
                self._sig.addParam(_pyNode.pySignature_ParameterUpdate.Scalar, type(value), value)

        self._edgesMeta = _pyNode.PyNodeEdgesMetaData([outDescriptor.id for outDescriptor in self._outDescriptors])
        self._nativeNode = self.create()
        self._func = FunctionParser.transformFunction(self._func, self)
        if self._isConcurrent:
            _pyNode.pyNode_UpCast(self._nativeNode).initWithConcurrent(self._funcAsByteArray, self._sig, self._edgesMeta)
        else:
            _pyNode.pyNode_UpCast(self._nativeNode).init(self._func, self._sig, self._edgesMeta)
        return OutDescriptorsTuple(self._outDescriptors)

    @property
    def outDescriptors(self):
        return self._outDescriptors

    @property
    def nativeNode(self):
        return self._nativeNode

    def create(self):
        return _pyNode.PyNodeFactory.create()

def unit(func):
    return UnitDescriptor(func, None, False)

def parallelUnit(func):
    functionAsByteArray = cloudpickle.dumps(func)
    return UnitDescriptor(func, functionAsByteArray, True)

#due to dependency, wait for unit to complete initialization
from graph_engine import GraphEngine
from wiring import Edge, OutDescriptor, OutDescriptorsTuple, idGenerator
