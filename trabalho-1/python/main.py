import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns
mpl.use('Agg')

data = np.genfromtxt("../inout/output.csv", delimiter=',')

size = int(np.sqrt(data.shape[0]))
data = data.reshape(size, size)

x = np.linspace(0, 0.05, size)
y = np.linspace(0, 0.1, size)

plt.figure(figsize=(8, 6))
ax = sns.heatmap(data, cmap='coolwarm', xticklabels=np.round(x, 3), yticklabels=np.round(y, 3), cbar_kws={'label': 'Valor'}, annot=False)

ax.invert_yaxis()

plt.savefig('../inout/heatmap.png', bbox_inches='tight')
plt.close()
