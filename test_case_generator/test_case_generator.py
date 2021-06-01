import networkx as nx
import random

N = 3900

G = nx.complete_graph(N)

f = open("cg3900.txt", "w")

f.write(str(N) + " " + str(len(G.edges())) + "\n")

for l in G.edges():
    f.write(str(l[0]+1) + " " + str(l[1]+1) + " " + str(random.randint(1, 10)) + "\n")

