import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
import numpy as np
import os

# Load the data
script_dir = os.path.dirname(os.path.abspath(__file__))
csv_path = os.path.join(script_dir, 'rosenbrock_data.csv')
data = pd.read_csv(csv_path)

x = data['x'].unique()
y = data['y'].unique()
X, Y = np.meshgrid(x, y)
Z = data['z'].values.reshape(len(x), len(y))

fig = plt.figure(figsize=(12, 8))
ax = fig.add_subplot(111, projection='3d')

# We use LogNorm because the 'walls' of the function are very steep
surf = ax.plot_surface(X, Y, Z, cmap='plasma', norm=LogNorm(), edgecolor='none', alpha=0.9)

ax.set_title('Benchmark Function #4: Rosenbrock (Banana) Function')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('f(x, y)')
fig.colorbar(surf, shrink=0.5, aspect=5)

# View from an angle that highlights the curved valley
ax.view_init(elev=35, azim=120)

plt.show()