import sys
from os.path import dirname, join
import inspect
from wiring import Edge
from unit_parser import FunctionParser

sys.path.append(join(dirname(__file__), '../../bin/'))
import _pyNode

class UnitOutDescriptorsTuple( tuple ):
    pass

class UnitDescriptor:
    def __init__(self, func):
        self._outDescriptors = []
        self.inScore = 0
        id = 0
        returnDescriptors = FunctionParser.getDescriptor(func)
        for returnDescriptor in returnDescriptors.values():
           self._outDescriptors.append(UnitOutDescriptor(id, returnDescriptor.name, returnDescriptor.type))
           id += 1

        self._func = FunctionParser.transformFunction(func)

    def __call__(self, *args, **kwargs):
        GraphEngine().addNode(self)

        funcSig = inspect.signature(self._func)
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

        self._sig = _pyNode.PyFunctionSignature()
        for value, param in zip(normalize_args,funcSig.parameters.values()):
            if type(param.default) == Edge:
                if not isinstance(value, UnitOutDescriptor):
                    raise ValueError('A non edge was connected to an edge, edge name - {0}'.format(param.name))
                value.addConsumer(self)
                value.type = param.default.type
                self._sig.addParam(_pyNode.pySignature_ParameterUpdate.Edge, value.type, value.id)
            else:
                if isinstance(value, UnitOutDescriptor):
                    raise ValueError('An edge was connected to a non edge type, param name - {0}'.format(param.name))
                self._sig.addParam(_pyNode.pySignature_ParameterUpdate.Scalar, type(value), value)

        for argName, value in kwargs.items():
            param = funcSig.parameters[argName]
            if type(param.default) == Edge:
                if not isinstance(value, UnitOutDescriptor):
                    raise ValueError('A non edge was connected to an edge, edge name - {0}'.format(param.name))
                value.addConsumer(self)
                value.type = param.default.type
                self._sig.addParam(_pyNode.pySignature_ParameterUpdate.Edge, value.type, value.id)
            else:
                if isinstance(value, UnitOutDescriptor):
                    raise ValueError('An edge was connected to a non edge type, param name - {0}'.format(param.name))
                self._sig.addParam(_pyNode.pySignature_ParameterUpdate.Scalar, type(value), value)

        return UnitOutDescriptorsTuple(self._outDescriptors)

    @property
    def outDescriptors(self):
        return self._outDescriptors

    def create(self):
        return _pyNode.PyNodeFactory.create(self._func, self._sig)

class UnitOutDescriptor:

    def __init__(self, id : int, name : str, type):
        self._id = id
        self._name = name
        self._type = type
        self._consumers = []

    def setType(self, type):
        if self._type == None:
            self._type = type
        elif self._type != type:
            raise ValueError('Inconsistency with edge data type, new declared type - {0}, original - {1}'.format(type, self._type))

    def getType(self):
        return self._type

    def addConsumer(self, consumer : UnitDescriptor):
        consumer.inScore += 1
        self._consumers.append(consumer)

    @property
    def id(self):
        return self._id

    @property
    def consumers(self):
        return self._consumers

    type = property(getType, setType)

def unit(func):
    return UnitDescriptor(func)
#due to dependency, wait for unit to complete initialization
from graph_engine import GraphEngine
