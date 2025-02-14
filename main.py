import matplotlib.pyplot as plt
import numpy as np
import csv
def display(csv_file_name):
    # Open the csv file
    with open(csv_file_name + '.csv', 'r') as f:
        data = list(csv.reader(f, delimiter=';'))

    data = np.array(data)
    arr = data.astype(float)
    arr[arr > 30] = 30  # account for noise values
    maxVal = np.amax(arr)
    arr = np.multiply(arr, 255/maxVal)
    matrix = arr.reshape(10, int(np.prod(arr.shape)/10))   # stretch the figure to create a square image
    matrix[1::2, :] = matrix[1::2, ::-1]
    plt.imshow(matrix, cmap='gray', vmin=0, vmax=255, aspect='auto')
    plt.show()

display('data386')