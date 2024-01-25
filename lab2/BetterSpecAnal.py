import numpy as np
import matplotlib.pyplot as plt
from PIL import Image

def BetterSpecAnal(x: np.ndarray, N: int, n: int):
    W = np.outer(np.hamming(N), np.hamming(N))

    h, w = x.shape

    h_begin = int((h - n * N) / 2)
    w_begin = int((w - n * N) / 2)

    res = np.zeros((N, N))
    for i in range(n):
        for j in range(n):
            t = W * x[h_begin + i * N: h_begin + (i+1) * N,
                      w_begin + j * N: w_begin + (j+1) * N]
            res += (1 / N**2) * np.abs(np.fft.fftshift(np.fft.fft2(t) ** 2)) * (1/n**2)

    return np.log(res)

if __name__ == "__main__":


    im = Image.open('img04g.tif')
    print('Read img04.tif.')
    print('Image size: ', im.size)

    # Import Image Data into Numpy array.
    # The matrix x contains a 2-D array of 8-bit gray scale values. 
    x = np.double(np.array(im))/255.0
    N, n = 64, 5

    # Plot the result using a 3-D mesh plot and label the x and y axises properly. 
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    a = b = np.linspace(-np.pi, np.pi, num = N)
    X, Y = np.meshgrid(a, b)

    PSD = BetterSpecAnal(x, N, n)

    surf = ax.plot_surface(X, Y, PSD, cmap=plt.cm.coolwarm)

    ax.set_xlabel('$\mu$ axis')
    ax.set_ylabel('$\\nu$ axis')
    ax.set_zlabel('Z Label')

    fig.colorbar(surf, shrink=0.5, aspect=5)

    plt.show()