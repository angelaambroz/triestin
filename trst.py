#!/usr/bin/python3 

import re

# Tokenizer/Lexer
class Token():
    def __init__(self, tipo, value):
        self.tipo = tipo
        self.value = value

    def __str__(self):
        return f"Token(type='{self.tipo}', value='{self.value}')"

class Tokenizer():
    def __init__(self, filename):
        self.file = filename

        # defining our language tokens
        # order matters! hence i'm using a tuple
        self.TOKEN_TYPES = (
            ('def', r'\bdef\b'), 
            ('fin', r'\bfin\b'),
            ('identifier', r'\b[a-zA-Z]+\b'),
            ('integer', r'\b[0-9]+\b'),
            ('oparen', r'\('),
            ('cparen', r'\)')
        )
        
        # the user-sourced code
        with open(filename, 'r') as f:
            self.code = f.read().replace('\n', '')
        
    def tokenize(self):
        tokens = []
        code = self.code
        tokens.append(self.tokenize_one_token(code, tokens))

        # Due to recursion, things got switched around - switching them back
        tokens_final = [x for x in tokens if x != None] 
        tokens_final.reverse()

        return tokens_final
                    
    def tokenize_one_token(self, chunk, list_of_tokens):
        """Now recursive!"""
        for (tipo, rgx_) in self.TOKEN_TYPES:
            z = re.findall(rgx_, chunk)
            if z:
                code = chunk.replace(z[0], '')
                token = Token(tipo, z[0])
                list_of_tokens.append(self.tokenize_one_token(code, list_of_tokens))
                return token

tokens = Tokenizer("trst.src").tokenize()
print(tokens)
for i in tokens:
    print(i)

# Parser
class DefNode():
    def __init__(self, name, arg_names, body):
        self.name = name
        self.arg_names = arg_names
        self.body = body

    def __str__(self):
        return f"DefNode(name='{self.name}', args='{self.arg_names}', body='{self.body}'"

class IntegerNode():
    def __init__(self, value):
        self.value = int(value)

    def __str__(self):
        return f"IntegerNode(value={self.value})"

class Parser():
    def __init__(self, list_of_tokens):
        self.tokens = list_of_tokens

    def parse(self):
        return self.parse_def()

    def parse_def(self):
        self.consume('def')
        name = self.consume('identifier').value
        arg_names = self.parse_arg_names()
        body = self.parse_expr()
        self.consume('fin')
        return DefNode(name, arg_names, body)

    def parse_arg_names(self):
        self.consume('oparen')
        # stuff in here
        args = []
        self.consume('cparen')
        return args

    def parse_expr(self):
        return self.parse_integer()

    def parse_integer(self):
        return IntegerNode(self.consume('integer').value)

    def consume(self, expected_type):
        token = self.tokens.pop(0)
        if token.tipo == expected_type:
            return token
        else:
            raise RuntimeError(f"Expected {expected_type}, got {token.tipo}.")



# tree = Parser(tokens).parse()
# print(tree)


# Code generator

# Compile to C, our definition


