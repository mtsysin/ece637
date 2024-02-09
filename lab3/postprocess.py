import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
import matplotlib as mpl
# Read in a segmentation TIFF image.
im = Image.open("ouptut.tif")
# Import Image Data into Numpy array.
x = np.array(im )
# Obtain number of segmentation area.
N = np.max(x)
# Randomly set color map.
cmap = mpl.colors.ListedColormap ( np . random . rand (N+1 ,3))
plt.imshow (x, cmap=cmap , interpolation="none")
plt.colorbar()
plt.title("Image")
plt.show()