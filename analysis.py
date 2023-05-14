# use a cli tool

import argparse

# parse the input file
parser = argparse.ArgumentParser(description='Postprocessing tool')

parser.add_argument('-i', '--input', type=str,
                    help='input file', default='input.txt')

parser.add_argument('-o', '--output', type=str,
                    help='output file', default='output.txt')

args = parser.parse_args()

data = []

"""
Data format looks like:

[
    [trial number, red, green, blue, colorTemp, lux, r, g, b, c]
]

"""
data = []


# read the input file
with open(args.input, 'r') as f:
    lines = f.readlines()
    for line in lines:
        split_line = line.split(",")
        obj = []

        # convert all values to floats
        for i in range(len(split_line)):
            obj.append(float(split_line[i]))

        data.append(obj)

chart = []

import matplotlib.pyplot as plt
import numpy as np
import matplotlib.cm as cm
from matplotlib.colors import LinearSegmentedColormap

colors = []
lux_values = []

for i in range(len(data)):
    red = data[i][1]
    green = data[i][2]
    blue = data[i][3]
    colors.append([red, green, blue])

    lux_values.append(data[i][5])

import colorsys

hues = []
for color in colors:
    # convert color to hue
    r, g, b = [x / 255.0 for x in color]

    h, _, _ = colorsys.rgb_to_hsv(r, g, b)

    hue = round(h * 360, 2)
    hues.append(hue)

fig = plt.figure(figsize=(10,6))

# convert colors to RGBA values
colors = np.array(colors) / 255.0

# clip
colors = np.clip(colors, 0, 1)

# create colorbar for x-axis
cmap = LinearSegmentedColormap.from_list("spectrum", colors)

hues = np.linspace(0, 360, len(hues))

# Plot lines connecting each point
plt.plot(hues, lux_values, color='black', linestyle='-', linewidth=1)

# Fill the entire column with corresponding color
for i in range(1, len(hues)):
    plt.fill_between([hues[i-1], hues[i]], [lux_values[i-1], lux_values[i]], color=colors[i])

plt.xlabel("Hue (degrees)")
plt.ylabel("Brightness (Lux)")

plt.title("Hue vs. Brightness")
plt.tight_layout()

plt.rcParams["font.family"] = "Times New Roman"

plt.grid(True, alpha=0.3)


plt.show()
