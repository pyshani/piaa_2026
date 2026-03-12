import matplotlib.pyplot as plt
import numpy as np

x = []
y = []

with open('times.txt', 'r') as file:
    for line in file:
        if line.strip():
            parts = line.split()
            if len(parts) >= 2:
                x.append(float(parts[0]))
                y.append(float(parts[1]))



x_t = np.linspace(1, 37, 37)
y_t = list(map(lambda x: float(2**x), x_t))

k = 0

for i, elem in enumerate(x):
    print(i, elem)
    k += y_t[int(elem)-1]/y[i]

k = k / len(x)


y_t = list(map(lambda x: x/k, y_t))

plt.figure(figsize=(8, 5))
plt.plot(x, y, color='r', label='Practic 2^n')
plt.plot(x_t, y_t, color='b', label='Theoretic 2^n')

plt.xlabel('x (простые числа)')
plt.ylabel('y (время)')
plt.title('График зависимости y от x')
plt.grid(True)
plt.legend()

plt.show()