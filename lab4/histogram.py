from matplotlib import pyplot as plt
import numpy as np
from PIL import Image
from matplotlib import cm

gray = cm.get_cmap("gray", 256)

def equalize(X, L = 256):
    hist = np.histogram(x.flatten(), bins = np.linspace(0, L - 1, L))[0]
    print(hist.shape)
    hist_cumsum = np.cumsum(hist)
    F = hist_cumsum / hist_cumsum[-1]
    Y = F[X]
    print(Y.shape, X.shape)

    return (L - 1) * (Y - np.min(Y)) / (np.max(Y) - np.min(Y)), F


# im = Image.open("race.tif")
# x = np.array(im)
# plt.imshow(x, cmap=gray)
# plt.show()

# plt.hist(x.flatten(), bins = np.linspace(0, 255, 256))
# plt.show()


im = Image.open("kids.tif")
x = np.array(im)
Z, F = equalize(x)

plt.imshow(x, cmap=gray)
plt.show()
plt.imshow(Z, cmap=gray)
plt.show()

plt.hist(x.flatten(), bins = np.linspace(0, 255, 256))
plt.show()
plt.hist(Z.flatten(), bins = np.linspace(0, 255, 256))
plt.show()


plt.plot(F)
plt.show()


