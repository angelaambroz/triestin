#!/usr/bin/python
# -*- coding: latin1 -*-
"""Parser
"""

from collections import namedtuple

# Structs
DefNode = namedtuple("DefNode", ("name", "arg_names", "body"))
IntegerNode = namedtuple("IntegerNode", ("val"))
CallNode = namedtuple("CallNode", ("name", "arg_exprs"))
VarRefNode = namedtuple("VarRefNode", ("val"))
AssignNode = namedtuple("AssignNode", ("name", "val"))
PrintNode = namedtuple("PrintNode", ("val"))
MathNode = namedtuple("MathNode", ("val"))
IdentifierNode = namedtuple("IdentifierNode", ("name", "arg_names"))
AnziNode = namedtuple("AnziNode", ("val"))


class Parser():
    def __init__(self, list_of_tokens):
        self.tokens = list_of_tokens

    def parse(self):
        if not self.peek('fin', -1):
            raise RuntimeError('Si deve sempre finire con `fin`!')
        if self.peek('def'):
            return self.parse_def()
        if self.peek('print'):
            return self.parse_print()
        if self.peek('integer') and self.peek('math',1):
            return self.parse_math()
        if self.peek('oparen') and self.peek('integer', 1):
            return self.parse_math()
        if self.peek('identifier') and self.peek('oparen', 1):
            return self.parse_call()
        if self.peek('identifier') and self.peek('assignment', 1):
            return self.parse_assignment()
        if self.peek('anzi'):
            return self.parse_anzi()
        if self.peek('identifier'):
            return self.parse_var_ref()
        else:
            raise RuntimeError(u"Non so proprio cos'è questo...")

    def parse_math(self):
        math_expr = ''
        while not self.peek('fin'):
            math_expr += self.consume_next().val
        self.consume('fin')
        return MathNode(math_expr)

    def parse_assignment(self):
        name = self.consume('identifier').val 
        self.consume('assignment')
        value = self.consume_next().val
        self.consume('fin')
        return AssignNode(name=name, val=value)

    def parse_anzi(self):
        """
        `anzi` is kind of like 'actually' or 'rather'
        it's kind of like "doch" (German) - it can signify "scratch that"

        programmatic `anzi` will revise whatever assignment you just made (for now)

        e.g. 
        x è 1 fin
        anzi 2 fin
        dimmi x fin
        >> 2 
        """
        self.consume('anzi')
        value = self.consume_next().val
        self.consume('fin')
        return AnziNode(val=value)

    
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
            raise RuntimeError("Aspettavo {}, ho ricevuto {}.".format(expected_type, token.tipo))

    def consume_next(self):
        return self.tokens.pop(0)

    def peek(self, expected_type, offset=0):
        return self.tokens[offset].tipo == expected_type
