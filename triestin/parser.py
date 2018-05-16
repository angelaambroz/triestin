"""Parser
"""

from collections import namedtuple

# Structs
DefNode = namedtuple("DefNode", ("name", "arg_names", "body"))
IntegerNode = namedtuple("IntegerNode", ("val"))
CallNode = namedtuple("CallNode", ("name", "arg_exprs"))
VarRefNode = namedtuple("VarRefNode", ("val"))
PrintNode = namedtuple("PrintNode", ("val"))
MathNode = namedtuple("MathNode", ("val"))

# Class
class Parser():
    def __init__(self, list_of_tokens):
        self.tokens = list_of_tokens

    def parse(self):
        if self.peek('def'):
            return self.parse_def()
        if self.peek('print'):
            return self.parse_print()
        else:
            print("HIIII")
    
    def parse_print(self):
        self.consume('print')
        string_to_print = []
        while not self.peek('fin'):
            string_to_print.append(self.consume_next().val)
        self.consume('fin')
        return PrintNode(" ".join(string_to_print))

    def parse_def(self):
        self.consume('def')
        name = self.consume('identifier').val
        arg_names = self.parse_arg_names()
        body = self.parse_expr()
        self.consume('fin')
        return DefNode(name, arg_names, body)

    def parse_arg_names(self):
        self.consume('oparen')
        # stuff in here
        args = []
        if self.peek('identifier'):
            args.append(self.consume('identifier').val)
            while self.peek('comma'):
                self.consume('comma')
                args.append(self.consume('identifier').val)
        self.consume('cparen')
        return args

    def parse_expr(self):
        if self.peek('integer'):
            return self.parse_integer()
        if self.peek('+') or self.peek('-'):
            return self.parse_math()
        if self.peek('print'):
            return self.parse_print()
        if self.peek('identifier') and self.peek('oparen', 1):
            return self.parse_call()
        else:
            return self.parse_var_ref()

    def parse_integer(self):
        return IntegerNode(int(self.consume('integer').val))

    def parse_call(self):
        name = self.consume('identifier').val
        arg_exprs = self.parse_arg_exprs()
        return CallNode(name, arg_exprs)

    def parse_arg_exprs(self):
        self.consume('oparen')
        # stuff in here
        arg_exprs = []
        if not self.peek('cparen'):
            arg_exprs.append(self.parse_expr())
            while self.peek('comma'):
                self.consume('comma')
                arg_exprs.append(self.parse_expr())
        self.consume('cparen')
        return arg_exprs

    def parse_var_ref(self):
        return VarRefNode(self.consume('identifier').val)

    def consume(self, expected_type):
        token = self.tokens.pop(0)
        if token.tipo == expected_type:
            return token
        else:
            raise RuntimeError(f"Expected {expected_type}, got {token.tipo}.")

    def consume_next(self):
        return self.tokens.pop(0)

    def peek(self, expected_type, offset=0):
        return self.tokens[offset].tipo == expected_type
