import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns
mpl.use('Agg')

data = np.genfromtxt("../inout/output.csv", delimiter=',')
size = np.sqrt(data.shape[0])
size = int(size)
data = data.reshape(size,size)

sns.heatmap(data=data, cmap="icefire")
plt.savefig('../inout/heatmap.png')
plt.close()