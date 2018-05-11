#!/usr/bin/python3 

import re

# Tokenizer/Lexer
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
        for (tipo, rgx) in self.TOKEN_TYPES:
            for chunk in self.code:
                z = re.findall(rgx_, chunk)
                if z:
                    print(z)




tokens = Tokenizer("trst.src").tokenize()



# Parser

# Code generator

# Compile to C, our definition