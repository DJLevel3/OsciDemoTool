import matplotlib.pyplot as plt
import cv2
import math

im = cv2.imread(input("Image filename? "))
im = cv2.cvtColor(im, cv2.COLOR_BGR2RGB)

implot = plt.imshow(im)
plt.axis('off')
impoints = plt.ginput(-1, 0)
plt.close()
dims = im.shape

dim = 256 / max(dims[0], dims[1])

points = []

print("Raw points:")

with open("points.txt", 'w') as f:
    for i in range(len(impoints)):
        x = int(math.floor(impoints[i][0] * dim - 128))
        y = int(math.floor(-(impoints[i][1] * dim - 128)))
        fString = '{' + f"{x}, {y}" + '},'
        print(fString)
        f.write(f"{fString}\n")
