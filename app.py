import numpy as np

# Load the first .npy file
file1 = np.load('output/ant_patches_3.npy')

# Load the second .npy file
file2 = np.load('output/ant_patches.npy')

# Compare the two arrays
if np.array_equal(file1, file2):
    print("The two .npy files are the same.")
else:
    print("The two .npy files are different.")