import matplotlib.pyplot as plt
import numpy as np
datos = np.loadtxt("datos.dat")

v = datos
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
# for i in lineas:
#     print(len(i))
#
x=np.array(range(256))
plt.figure(figsize=(10,10))
plt.imshow(v)
# plt.streamplot(x,x,Ex,Ey)
plt.xlim((0,256))
plt.ylim((256,0))
plt.savefig("placas.pdf")
