import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns
mpl.use('Agg')

data = np.genfromtxt("../inout/output.csv", delimiter=',')
data = data.reshape(98,98)

sns.heatmap(data=data, cmap="icefire")
plt.savefig('../inout/heatmap.png')
plt.close()