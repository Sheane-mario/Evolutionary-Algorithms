import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
import numpy as np
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
csv_path = os.path.join(script_dir, 'penalty1_data.csv')
data = pd.read_csv(csv_path)

x = data['x'].unique()
y = data['y'].unique()
X, Y = np.meshgrid(x, y)
Z = data['z'].values.reshape(len(x), len(y))

fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection='3d')

# Using LogNorm because the penalty terms create massive 'walls' at the edges
surf = ax.plot_surface(X, Y, Z, cmap='inferno', norm=LogNorm(), edgecolor='none')

ax.set_title('Benchmark Function #7: Penalty Function #1')
ax.set_xlabel('X1')
ax.set_ylabel('X2')
ax.set_zlabel('f(x1, x2)')
plt.show()