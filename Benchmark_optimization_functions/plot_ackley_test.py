import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import os

# Load the data
script_dir = os.path.dirname(os.path.abspath(__file__))
csv_path = os.path.join(script_dir, 'ackley_test_data.csv')
data = pd.read_csv(csv_path)

x = data['x'].unique()
y = data['y'].unique()
X, Y = np.meshgrid(x, y)
Z = data['z'].values.reshape(len(x), len(y))

fig = plt.figure(figsize=(12, 8))
ax = fig.add_subplot(111, projection='3d')

# Render the surface with a distinct color map
surf = ax.plot_surface(X, Y, Z, cmap='magma', edgecolor='none', alpha=0.9)

ax.set_title('Benchmark Function #3: Ackley Test Function')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('f(x, y)')
fig.colorbar(surf, shrink=0.5, aspect=5)

plt.show()