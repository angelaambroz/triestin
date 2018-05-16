#!/usr/bin/python3 

from triestin import Tokenizer, Parser, Generator

tokens = Tokenizer("trst.src").tokenize()
# for i in tokens:
#     print(i)
tree = Parser(tokens).parse()
# print(tree)
generated = Generator().generate(tree)
print(generated)

