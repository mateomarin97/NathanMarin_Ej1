import matplotlib.pyplot as plt
import numpy as np
datos= np.loadtxt("datos.dat")
v=[]
v = datos
# Ex=[]
# Ey=[]
# for i in range(250):
# v.append(datos[:,i])
# Ex.append(datos[:,i+250])
# Ey.append(datos[:,i+(250*2)])
# v=np.transpose(v)
# Ex=np.transpose(Ex)
# Ey=np.transpose(Ey)
# Ex=np.array(Ex)
# Ey=np.array(Ey)
x=np.array(range(256))
plt.figure(figsize=(10,10))
plt.imshow(v)
# plt.streamplot(x,x,Ex,Ey)
plt.xlim((0,256))
plt.ylim((256,0))
plt.savefig("placas.pdf")

    

