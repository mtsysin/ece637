from matplotlib import pyplot as plt
import numpy as np
from PIL import Image
from matplotlib import cm

gray = cm.get_cmap("gray", 256)

def stretch(input, T1, T2):
    output = np.zeros(input.shape)
    mask = np.bitwise_and((T1 < input), (input < T2))
    output[mask] = (input[mask] - T1) / (T2 - T1) * 255
    output[input > T2] = 255
    return np.uint8(output)



im = Image.open("kids.tif")
x = np.array(im)
Y = stretch(x, 70, 180)

plt.imshow(x, cmap=gray)
plt.show()
plt.imshow(Y, cmap=gray)
plt.show()

plt.hist(x.flatten(), bins = np.linspace(0, 255, 256))
plt.show()
plt.hist(Y.flatten(), bins = np.linspace(0, 255, 256))
plt.show()