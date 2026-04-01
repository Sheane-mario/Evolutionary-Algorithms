import os
import matplotlib.pyplot as plt
import pandas as pd

script_dir = os.path.dirname(os.path.abspath(__file__))
csv_path = os.path.join(script_dir, 'exercise_2_11_data.csv')
data = pd.read_csv(csv_path)

plt.figure(figsize=(10, 6))
plt.plot(data['pm'], data['avg_fitness'], marker='o', linestyle='-', color='teal')
plt.title("Monte Carlo Simulation: Average Fitness vs. Mutation Probability")
plt.xlabel('Mutation Probability (pm)')
plt.ylabel('Average Fitness')
plt.grid(True, alpha=0.3)
plt.show()