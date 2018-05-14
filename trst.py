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
            self.code = [x.replace('\n', '') for x in f.readlines()]
        
    def tokenize(self):
        tokens = []
        for chunk in self.code:
            tokens.append(self.tokenize_one_token(chunk))

        return tokens
                    
    def tokenize_one_token(self, chunk):
        for (tipo, rgx_) in self.TOKEN_TYPES:
            z = re.findall(rgx_, chunk)
            if z:
                return Token(tipo, z[0])

tokens = Tokenizer("trst.src").tokenize()
for i in tokens:
    print(i)

# Parser




# Code generator

# Compile to C, our definition