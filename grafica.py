import matplotlib.pyplot as plt
import numpy as np
with open('datos.dat') as f:
    lines = f.readlines()

datos = lines[0].split(",")

palabras = []
lineas = []
# v = datos
# Ex=[]
# Ey=[]
# for i in range(256):
# 	v.append(datos[:,i])
# 	Ex.append(datos[:,i+256])
# 	Ey.append(datos[:,i+(256*2)])
# v=np.transpose(v)
# Ex=np.transpose(Ex)
# Ey=np.transpose(Ey)
# Ex=np.array(Ex)
# Ey=np.array(Ey)
for i in range(256):
    palabras = []
    for j in range(256):
        print(i , j)
        palabras.append(float(datos[i*256 + j]))
    lineas.append(palabras)
# for i in lineas:
#     print(len(i))
#
x=np.array(range(256))
plt.figure(figsize=(10,10))
plt.imshow(lineas)
# plt.streamplot(x,x,Ex,Ey)
plt.xlim((0,256))
plt.ylim((256,0))
plt.savefig("placas.pdf")
