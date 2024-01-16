import numpy as np
from functools import lru_cache
from PIL import Image
import matplotlib.pyplot as plt
    

if __name__ == "__main__":

    # Theoretical transfer function
    def H(mu, nu):
        return 1 / 81 * np.abs(np.sin(9*mu/2) * np.sin(9*nu/2) / np.sin(mu/2) /np.sin(nu/2))
    
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
    fig.savefig("lab1/H_FIR.tif")
    fig.show()
