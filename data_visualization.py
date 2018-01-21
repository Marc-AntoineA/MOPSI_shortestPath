import pandas as pd
from matplotlib import pyplot as plt

# Affichage des points souhaités sur le graphe

cG = pd.read_csv("Code/Instance/NewYork/coords.csv", sep = " ")
cG.head()

landmarks = [60837, 191190, 215369, 34818, 135719, 104859, 40103, 172432, 28563,88354, 57946, 117420, 43137, 254114, 133650]
int(cG.loc[cG['id'] == 1].Lon)

points = [246747, 175096]

X_p = [int(cG.loc[cG['id'] == x].Lon) for x in points]
Y_p = [int(cG.loc[cG['id'] == x].Lat) for x in points]

X_l = [int(cG.loc[cG['id'] == x].Lon) for x in landmarks]
Y_l = [int(cG.loc[cG['id'] == x].Lat) for x in landmarks]


plt.scatter(X_p, Y_p, c = "r")
plt.scatter(X_l, Y_l, c = "b")
plt.show()
plt.savefig("i.png")

dist = pd.read_csv("Code/Instance/NewYork/dist.csv", sep = " ")
len(dist)
dist
dist.ix[0]

for x in range(len(dist)//2):
    if mod(x, 1000) == 0:
        print(x)
    if dist.ix[2*x].dist != dist.ix[2*x + 1].dist:
        break
