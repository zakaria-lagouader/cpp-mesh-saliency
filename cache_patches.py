from multiprocessing import Pool, cpu_count
import numpy as np
import sys

PATCH_SIZE = 32
TARGET = np.array([0, 1, 0])
I2HC = np.load("./cached_variables/I2HC.npy")
HC2I = np.load("./cached_variables/HC2I.npy")

def rotatePatch(patch, axis, theta):
    if theta != 0:
        I = np.eye(3)
        K = np.array([[0, -axis[2], axis[1]], [axis[2], 0, -axis[0]], [-axis[1], axis[0], 0]])
        R = I + np.sqrt(1 - (theta * theta)) * K + (1 - theta) * np.matmul(K, K)
        for fIndex in range(np.shape(patch)[0]):
            patch[fIndex,] = np.matmul(R, np.transpose(patch[fIndex,]))
    patch = np.transpose(patch)

    return patch

def computeRotation(vec, target):
    theta = (np.dot(vec, target) / (np.linalg.norm(vec) * np.linalg.norm(target)))
    axis_pre = np.cross(vec, target)
    if np.linalg.norm(axis_pre) != 0:
        axis = axis_pre / np.linalg.norm(axis_pre)

    if np.linalg.norm(axis_pre) == 0:
        axis_pre = np.cross(vec, np.roll(target, 1))
        axis = axis_pre / np.linalg.norm(axis_pre)

    return axis, theta

def process_patch(normalsPatch):
    vec = np.mean(normalsPatch, axis=0)
    vec = vec / np.linalg.norm(vec)
    axis, theta = computeRotation(vec, TARGET)
    normalsPatch = rotatePatch(normalsPatch, axis, theta)
    normalsPatchR = normalsPatch.reshape((PATCH_SIZE, PATCH_SIZE, 3))
    for hci in range(I2HC.shape[0]):
        normalsPatchR[I2HC[hci, 0], I2HC[hci, 1], :] = normalsPatch[:, HC2I[I2HC[hci, 0], I2HC[hci, 1]]]
    return (normalsPatchR + 1.0 * np.ones(np.shape(normalsPatchR))) / 2.0

def process_model(patches_path, normals_path, output_path):
    print("Caching patches...")
    print("loading patches from:", patches_path)
    patches = np.loadtxt(patches_path, delimiter=',', dtype=int)
    patches = patches[:, 1:]

    print("loading normals from:", normals_path)
    patchFacesNormals = np.loadtxt(normals_path, delimiter=',')

    print("Processing patches...")
    with Pool(cpu_count()) as pool:
        data = pool.map(process_patch, [patchFacesNormals[p] for p in patches])

    print("Caching patches...")
    np.save(output_path, data)
    print("Patches cached successfully!")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python cache_patches.py <patches_path> <normals_path> <output_path>")
        sys.exit(1)

    patches_path = sys.argv[1]
    normals_path = sys.argv[2]
    output_path = sys.argv[3]

    process_model(patches_path, normals_path, output_path)