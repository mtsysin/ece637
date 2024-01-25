import numpy as np
import matplotlib.pyplot as plt   
from BetterSpecAnal import BetterSpecAnal

N = 512
x = np.random.uniform(-0.5, 0.5, (N, N))
x_scaled = 255 * (x + 0.5)

plt.imshow(np.uint8(x_scaled), cmap=plt.cm.gray)
plt.show()

y = np.zeros((N, N))

for i in range(N):
    for j in range(N):
        y[i, j] += 3 * x[i, j]
        if i > 0:
            y[i, j] += 0.99 * y[i-1, j]
        if j > 0:
            y[i, j] += 0.99 * y[i, j-1]
        if i > 0 and j > 0:
            y[i, j] -= 0.9801 * y[i-1, j-1]
print(np.max(y))

plt.imshow(np.uint8(np.clip(y + 127, 0, 255)), cmap=plt.cm.gray)
plt.show()


s = BetterSpecAnal(y, 64, 5)
U, V = np.meshgrid(np.linspace(-np.pi, np.pi, 64), np.linspace(-np.pi, np.pi, 64))
print (U, V)
theoretical_s = (1/12) *  np.abs(3 / ((1 - 0.99 * np.exp(-1j * U)) * (1 - 0.99 * np.exp(-1j * V)))) ** 2

# Plot the result using a 3-D mesh plot and label the x and y axises properly. 
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
a = b = np.linspace(-np.pi, np.pi, num = 64)
X, Y = np.meshgrid(a, b)

surf = ax.plot_surface(X, Y, s, cmap=plt.cm.coolwarm)

ax.set_xlabel('$\mu$ axis')
ax.set_ylabel('$\\nu$ axis')
ax.set_zlabel('Z Label')

fig.colorbar(surf, shrink=0.5, aspect=5)

plt.show()


# Plot the result using a 3-D mesh plot and label the x and y axises properly. 
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
a = b = np.linspace(-np.pi, np.pi, num = 64)
X, Y = np.meshgrid(a, b)

surf = ax.plot_surface(X, Y, np.log(theoretical_s), cmap=plt.cm.coolwarm)

ax.set_xlabel('$\mu$ axis')
ax.set_ylabel('$\\nu$ axis')
ax.set_zlabel('Z Label')

fig.colorbar(surf, shrink=0.5, aspect=5)

plt.show()