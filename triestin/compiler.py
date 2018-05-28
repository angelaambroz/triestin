#!/usr/bin/python
# -*- coding: latin1 -*-
"""Compiler
"""

import pprint
from .parser import DefNode, IntegerNode, CallNode, VarRefNode, PrintNode, MathNode, AssignNode, AnziNode
from .state import State


class Generator():
    def __init__(self):
        return None

    # def save_state(self, *args):
    #     x, y = self.generate(*args)
    #     self.local_state.add_history(x)
    #
    # @save_state
    def generate(self, node):
        """La gloria e il dramma
        """
        if type(node) == DefNode:
            return {'node': type(node).__name__,
                    'code': "def {}({}):\n return {}\n".format(node.name,
                            ','.join(node.arg_names), self.generate(node.body)['code']),
                    'state': 'exec'}
        if type(node) == CallNode:
            return {'node': type(node).__name__,
                    'code': "{}({})".format(node.name,
                                    ','.join([self.generate(expr)['code'] for expr in node.arg_exprs])),
                    'state': 'eval'}
        if type(node) == AssignNode:
            return {'node': type(node).__name__,
                    'code': "{} = {}".format(node.name, node.val),
                    'state': 'exec'}
        if type(node) == VarRefNode:
            return {'node': type(node).__name__,
                    'code': node.val,
                    'state': 'eval'}
        if type(node) == IntegerNode:
            return {'node': type(node).__name__,
                    'code': str(node.val),
                    'state': 'eval'}
        if type(node) == PrintNode:
            return {'node': type(node).__name__,
                    'code': "print('{}')".format(node.val),
                    'state': 'exec'}
        if type(node) == MathNode:
            node_name = str(node)
            return {'node': type(node).__name__,
                    'code': "{}".format(node.val),
                    'state': 'eval'}
        if type(node) == AnziNode:
            return {'node': type(node).__name__,
                    'code': node.val,
                    'state': 'anzi'}
        else:
            return RuntimeError("Nodo tipo inaspettato {}".format(node))
