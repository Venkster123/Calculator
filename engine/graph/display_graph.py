from matplotlib.animation import FuncAnimation

import matplotlib.pyplot as plt
import pandas
import os
import sys

args = sys.argv

if len(args) != 2:
    print("Incorrect number of arguments, terminating...")

    sys.exit(-1)

gpath = sys.argv[1]

def animate(k):
    plt.cla()

    data = pandas.read_csv(gpath)

    x = data['epoch']
    y1 = data['accuracy']
    y2 = data['loss']

    plt.plot(x, y1, label="Accuracy")
    plt.plot(x, y2, label="Loss")

    plt.xlabel("Epoch")
    plt.title("Training Statistics")

    plt.legend()

    plt.tight_layout()

anim = FuncAnimation(plt.gcf(), animate, interval=1000)

plt.tight_layout()
plt.show()

os.remove(gpath)
