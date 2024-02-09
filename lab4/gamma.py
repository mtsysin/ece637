from matplotlib import pyplot as plt
import numpy as np
from PIL import Image
from matplotlib import cm

gray = cm.get_cmap("gray", 256)


def get_checkerboard(num_stripes = 5, L = 100, M = 10):
    checkerboard_row = (-1) ** np.arange(L)
    checkerboard_col = (-1) ** np.arange(M)
    checkerboard = np.reshape(checkerboard_col, (M, 1)) @ np.reshape(checkerboard_row, (1, L))

    gray = np.zeros((M, L)) + 0.5

    element = np.concatenate([checkerboard, gray], axis=0)
    print(element.shape)
    return np.repeat(element, (num_stripes, 1), axis = 0)


checkerboard = get_checkerboard()

plt.imshow(checkerboard, cmap=gray)
plt.show()