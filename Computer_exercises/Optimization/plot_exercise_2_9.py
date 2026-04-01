import os
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def f(x): 
    return x**3 + 4*x**2 - 4*x + 1

script_dir = os.path.dirname(os.path.abspath(__file__))
csv_path = os.path.join(script_dir, 'exercise_2_9_data.csv')
data = pd.read_csv(csv_path)

x = data['x']
y = data['y']

plt.figure(figsize=(10, 6))
plt.plot(x, y, label='f(x)=x^3 + 4x^2 - 4x + 1')

plt.scatter([0.43], [f(0.43)], color='red', label='Local Minimum (~0.43)')
plt.scatter([-3.1], [f(-3.1)], color='orange', label='Local Maximum (~-3.1)')

plt.scatter([-5], [-4], color='blue', marker='x', s=100, label='Global Minimum (Boundary x=-5)')
plt.scatter([3], [52], color='green', marker='x', s=100, label='Global Maximum (Boundary x=3)')


plt.title("Exercise 2.9: Plot of f(x) = x^3 + 4x^2 - 4x + 1");
plt.xlabel('x')
plt.ylabel('f(x)')
plt.grid(True)
plt.legend()
plt.show()