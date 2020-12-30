# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import random
from matplotlib import cm
from matplotlib import axes


def drawPlotFRR():
    cycleData = np.arange(10).tolist()
    StandardDeviationData0 = [0]*10
    StandardDeviationData0[0] = 0.928
    StandardDeviationData0[1] = 0.836
    StandardDeviationData0[2] = 0.680
    StandardDeviationData0[3] = 0.403

    plt.figure(dpi=600)
    plt.minorticks_on()
    plt.plot(cycleData, StandardDeviationData0, 'b^-', linewidth=1)
#     plt.title('FRR')
    plt.xlabel('thresh hold')
    plt.ylabel('FRR')

#     plt.text(0-0.3, StandardDeviationData0[0]+0.05, '0.928')
#     plt.text(1-0.3, StandardDeviationData0[1]+0.05, '0.836')
#     plt.text(2-0.3, StandardDeviationData0[2]+0.05, '0.680')
#     plt.text(3-0.35, StandardDeviationData0[3]+0.02, '0.403')
#     plt.legend()

    plt.ylim(ymax=1.1)
    plt.grid()

#     if len(saveName) != 0:
#         plt.savefig(saveName)
#     else:

    plt.savefig("FRR")
#     plt.show()


def drawPlotFARByDiffFinger():
    cycleData = np.arange(160).tolist()
    StandardDeviationData0 = [1]*160

    for idx in range(32):
        StandardDeviationData0[idx] = 0

    StandardDeviationData0[32] = 0.194/7
    StandardDeviationData0[33] = 0.416/7
    StandardDeviationData0[34] = 0.642/7
    StandardDeviationData0[35] = 0.852/7
    StandardDeviationData0[36] = 1.0/7
    StandardDeviationData0[37] = 1.0/7
    StandardDeviationData0[38] = 1.0/7
    StandardDeviationData0[39] = 1.0/7
    StandardDeviationData0[40] = 1.2/7
    StandardDeviationData0[41] = 2.384/7
    StandardDeviationData0[42] = 4.288/7
    StandardDeviationData0[43] = 6.126/7
    StandardDeviationData0[44] = 6.922/7

    StandardDeviationData0 = StandardDeviationData0[:1] + \
        StandardDeviationData0[31:46] + StandardDeviationData0[159:]
    cycleData = cycleData[:1] + cycleData[31:46] + cycleData[159:]

    print(cycleData)
    print(len(StandardDeviationData0), ",", len(cycleData))

    plt.figure(dpi=600)
    plt.minorticks_on()
    plt.plot(cycleData, StandardDeviationData0, 'bo-', linewidth=1)
    plt.title('(a)')
    plt.xlabel('thresh hold')
    plt.ylabel('FAR')
    plt.grid()

#     plt.legend()

    plt.ylim(ymax=1.1)

    plt.savefig("FARByDiffFinger")
#     plt.show()


def drawPlotFAR():
    cycleData = np.arange(160).tolist()
    StandardDeviationData0 = [1]*160
    cycleData1 = cycleData[:]
    StandardDeviationData1 = StandardDeviationData0[:]

    for idx in range(32):
        StandardDeviationData0[idx] = 0

    for idx in range(66):
        StandardDeviationData1[idx] = 0
    StandardDeviationData1[66] = 0.5

    StandardDeviationData0[32] = 0.194/7
    StandardDeviationData0[33] = 0.416/7
    StandardDeviationData0[34] = 0.642/7
    StandardDeviationData0[35] = 0.852/7
    StandardDeviationData0[36] = 1.0/7
    StandardDeviationData0[37] = 1.0/7
    StandardDeviationData0[38] = 1.0/7
    StandardDeviationData0[39] = 1.0/7
    StandardDeviationData0[40] = 1.2/7
    StandardDeviationData0[41] = 2.384/7
    StandardDeviationData0[42] = 4.288/7
    StandardDeviationData0[43] = 6.126/7
    StandardDeviationData0[44] = 6.922/7

    StandardDeviationData0 = StandardDeviationData0[:1] + \
        StandardDeviationData0[31:47] + StandardDeviationData0[159:]
    cycleData = cycleData[:1] + cycleData[31:47] + cycleData[159:]

    StandardDeviationData1 = StandardDeviationData1[:1] + \
        StandardDeviationData1[66:68] + StandardDeviationData1[159:]
    cycleData1 = cycleData1[:1] + \
        cycleData1[66:68] + cycleData1[159:]

    print(StandardDeviationData0)
    print(len(StandardDeviationData0), ",", len(cycleData))

    plt.figure(dpi=600)
    plt.minorticks_on()
    line1 = plt.plot(cycleData, StandardDeviationData0, 'ro-',
                     label='different finger,same device')
    line2 = plt.plot(cycleData1, StandardDeviationData1, 'g+-',
                     label='same finger,different device')
    plt.plot(cycleData, StandardDeviationData0, 'ro-',
             cycleData1, StandardDeviationData1, 'g+-',
             )
    plt.xlabel('thresh hold')
    plt.ylabel('FAR')
    plt.grid()

    plt.legend()

    plt.ylim(ymax=1.1)
    plt.xlim(xmax=80)

    plt.savefig("FAR")
#     plt.show()


def drawBarHamingRate(edgecache_dp_data,  total_write_cycles, save):

    #     name_list = ['Monday', 'Tuesday', 'Friday', 'Sunday']
    name_list = list(range(160))
    for name in name_list:
        name = str(name)
    num_list = [1.5, 0.6, 7.8, 6]
    num_list1 = [1, 2, 3, 1]
    x = list(range(160))
    total_width, n = 0.8, 2
    width = total_width / n

    edgecache_dp_data = edgecache_dp_data[:160]

    fig = plt.figure(dpi=600)
    ax = fig.add_subplot(111)
    ax.bar(x, edgecache_dp_data,  label='boy')
    tick_spacing = 20  # 通过修改tick_spacing的值可以修改x轴的密度
    ax.xaxis.set_major_locator(ticker.MultipleLocator(tick_spacing))
    ax.set_xlabel("Hamming Distance")
    ax.set_ylabel("Distribution Probility")

#     plt.legend()
#     plt.show()
    plt.savefig('rspHammingDist')


def main():
    #     drawPlotFRR()
    #     drawPlotFAR()
    finger_haming_rate = [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.01, 0.00, 0.01, 0.00, 0.00, 0.00, 0.00, 0.04, 0.00, 0.00, 0.00, 0.00, 0.03, 0.02, 0.02, 0.02, 0.02, 0.02, 0.03, 0.01, 0.01, 0.07, 0.07, 0.05, 0.02, 0.01, 0.08, 0.02, 0.04, 0.03, 0.02, 0.00, 0.04, 0.01, 0.07, 0.01, 0.00, 0.02, 0.04, 0.02, 0.00, 0.02, 0.01, 0.01, 0.05, 0.01, 0.01, 0.00, 0.00, 0.02, 0.00, 0.00, 0.01, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                          0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00]
    resp_haming_rate = [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.01, 0.00, 0.02, 0.02, 0.05, 0.04, 0.03, 0.05, 0.08, 0.08, 0.08, 0.13, 0.05, 0.08, 0.05, 0.08, 0.06, 0.02, 0.02, 0.02, 0.01, 0.01, 0.00, 0.00, 0.01, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                        0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00]
    drawBarHamingRate(
        resp_haming_rate,  100, save=True)


if __name__ == "__main__":
    main()
