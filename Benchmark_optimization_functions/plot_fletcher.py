import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
csv_path = os.path.join(script_dir, 'fletcher_data.csv')
data = pd.read_csv(csv_path)

x = data['x'].unique()
y = data['y'].unique()
X, Y = np.meshgrid(x, y)
Z = data['z'].values.reshape(len(x), len(y))

fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection='3d')
surf = ax.plot_surface(X, Y, Z, cmap='gist_earth', edgecolor='none')

ax.set_title('Benchmark Function #5: Fletcher Function')
ax.set_xlabel('X1')
ax.set_ylabel('X2')
ax.set_zlabel('f(x1, x2)')
plt.show()