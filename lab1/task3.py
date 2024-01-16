import numpy as np
from functools import lru_cache
from PIL import Image
import matplotlib.pyplot as plt

H, W = 265, 265

@lru_cache(None)
def y(i, j):
    if (i < 0) or (j < 0):
        return 0
    return 0.01 * (1 if (i == 127 and j == 127) else 0) + 0.9 * (y(i-1, j) + y(i, j-1)) - 0.81*y(i-1, j-1)
    

if __name__ == "__main__":
    h = np.zeros([256, 256])
    for i in range(256):
        for j in range(256):
            h[i,j] = y(i, j)
    im_save = Image.fromarray((255*100*h).astype(np.uint8))
    im_save.save("lab1/h_out.tif")
    plt.imshow(h, cmap=plt.cm.gray)

    # Theoretical transfer function
    def H(mu, nu):
        return np.abs(0.1/(1 - 0.9 * np.exp(-1j * mu)) * 0.1 / (1 - 0.9 * np.exp(-1j * nu)))
    
    mu = np.linspace(-np.pi, np.pi, 256)
    nu = np.linspace(-np.pi, np.pi, 256)
    mu, nu = np.meshgrid(mu, nu)

    H_mu_nu = H(mu, nu)
    # Plot the 2D function
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')
    contour = ax.plot_surface(mu, nu, H_mu_nu, cmap='viridis')  # Contour plot with filled regions
    fig.colorbar(contour)  # Add a colorbar for reference
    ax.set_title('IIR filter')
    ax.set_xlabel(r'$\mu$')
    ax.set_ylabel(r'$\nu$')
    ax.set_zlabel(r'$H(\mu, \nu)$')
    fig.savefig("lab1/H_IIR.tif")
    fig.show()