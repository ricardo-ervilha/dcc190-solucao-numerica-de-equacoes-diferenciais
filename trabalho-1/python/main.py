import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns
mpl.use('Agg')

data = np.genfromtxt("../inout/exercise.csv", delimiter=',')

size = int(np.sqrt(data.shape[0]))
u = data.reshape(size, size)

h = 0.05
x = np.arange(0, 1+h, h)
y = np.arange(0, 1+h, h)

# print(u)

X, Y = np.meshgrid(x, y)
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.plot_wireframe(X, Y, u)

plt.ylabel('y')
plt.xlabel('x')

plt.savefig('../inout/exercise.png')