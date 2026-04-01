import os
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

script_dir = os.path.dirname(os.path.abspath(__file__))
csv_path = os.path.join(script_dir, 'exercise_2_10_data.csv')
df = pd.read_csv(csv_path)

# Implementation of the mathematical concept of Pareto optimality (Dominance)
def is_pareto_optimal(costs):
    """Returns a boolean array indicating whether each point is Pareto optimal."""
    is_optimal = np.ones(costs.shape[0], dtype=bool)
    for i, c in enumerate(costs):
        for j, c_p in enumerate(costs):
            if (i==j):
                continue
            if all(c_p <= c) and any(c_p < c):
                is_optimal[i] = False
                break
    return is_optimal

# Identify Pareto Front 
costs = df[['f1', 'f2']].values
pareto_mask = is_pareto_optimal(costs)
pareto_front = df[pareto_mask].sort_values(by='f1')

plt.figure(figsize=(12, 6))

# --- Pareto Front in Objective Space (f1, f2) ---
plt.subplot(1, 2, 1)
plt.scatter(df['f1'], df['f2'], color='lightgray', s=10, label='Dominated Points ')
plt.scatter(pareto_front['f1'], pareto_front['f2'], color='red', s=30, label='Pareto Front')
plt.plot(pareto_front['f1'], pareto_front['f2'], linestyle='--', color='red', alpha=0.5)
plt.title("Objective Space: Pareto Front")
plt.xlabel('f1(x1,x2)')
plt.ylabel('f2(x1,x2)')
plt.legend()
plt.grid(True)

# --- Pareto Set in Decision Space (x1, x2) ---
plt.subplot(1, 2, 2)
plt.scatter(df['x1'], df['x2'], color='lightgray', s=10, label='Search Space')
plt.scatter(pareto_front['x1'], pareto_front['x2'], color='blue', s=30, marker='s', label='Pareto Set', alpha=0.4)
plt.title("Decision Space: Pareto Set")
plt.xlabel('x1')
plt.ylabel('x2')
plt.legend()
plt.grid(True)


plt.tight_layout()
plt.show()

print("Pareto Optimal Points: (x1, x2, f1, f2)")
print(df[pareto_mask][['x1', 'x2', 'f1', 'f2']])