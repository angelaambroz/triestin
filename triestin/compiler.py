"""Compiler
"""

from .parser import DefNode, IntegerNode, CallNode, VarRefNode, PrintNode, MathNode


class Generator():
    def __init__(self):
        return None

    def generate(self, node):
        """The glory and drama of exec() vs. eval() vs. ...compile()?
        """

        if type(node) == DefNode:
            return "def {}({}):\n return {}\n".format(node.name, 
                ','.join(node.arg_names), self.generate(node.body)), 'exec'
        if type(node) == CallNode:
            return "{}({})".format(node.name, 
                ','.join([self.generate(expr) for expr in node.arg_exprs])), 'exec'
        if type(node) == VarRefNode:
            return node.val, 'eval'
        if type(node) == IntegerNode:
            return str(node.val), 'eval'
        if type(node) == PrintNode:
            return "print('{}')".format(node.val), 'exec'
        if type(node) == MathNode:
            return "{}".format(node.val), 'eval'
        else:
            raise RuntimeError("Nodo tipo inaspettato {}".format(node))


