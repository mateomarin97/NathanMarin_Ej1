import numpy as np


with open('datos.dat') as f:
    lines = f.readlines()

for i in lines:
    print(len(i.split(" ")))
