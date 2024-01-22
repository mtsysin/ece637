import numpy as np
from functools import lru_cache
from PIL import Image
import matplotlib.pyplot as plt
    

if __name__ == "__main__":

    # Theoretical transfer function
    def H(mu, nu):
        return 1 / 25 * np.sin(5*mu/2) * np.sin(5*nu/2) / np.sin(mu/2) / np.sin(nu/2)
    
    def G(mu, nu):
        return 1 + 1.5 * (1 - H(mu, nu))
    
    mu = np.linspace(-np.pi, np.pi, 256)
    nu = np.linspace(-np.pi, np.pi, 256)
    mu, nu = np.meshgrid(mu, nu)

    H_mu_nu = H(mu, nu)
    G_mu_nu = G(mu, nu)

    # Plot the 2D function
    figH = plt.figure(figsize=(10, 8))
    axH = figH.add_subplot(111, projection='3d')
    contour = axH.plot_surface(mu, nu, np.abs(H_mu_nu), cmap='viridis')  # Contour plot with filled regions
    figH.colorbar(contour)  # Add a colorbar for reference
    # axH.set_title('LPF filter')
    axH.set_xlabel(r'$\mu$')
    axH.set_ylabel(r'$\nu$')
    axH.set_zlabel(r'$H(\mu, \nu)$')
    figH.savefig("lab1/H_FIR_SHARP.tif")
    figH.show()

    figG = plt.figure(figsize=(10, 8))
    axG = figG.add_subplot(111, projection='3d')
    contour = axG.plot_surface(mu, nu, np.abs(G_mu_nu), cmap='viridis')  # Contour plot with filled regions
    figG.colorbar(contour)  # Add a colorbar for reference
    # axG.set_title('HPF filter')
    axG.set_xlabel(r'$\mu$')
    axG.set_ylabel(r'$\nu$')
    axG.set_zlabel(r'$G(\mu, \nu)$')
    figG.savefig("lab1/G_SHARP.tif")
    figG.show()
