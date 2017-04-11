import matplotlib.pyplot as plt
import numpy as np

# Create random data with numpy
import numpy as np

with open("largest_fitness.dat") as lf:
    l_fit = lf.readlines()

with open("average_fitness.dat") as af:
    a_fit = af.readlines()

gen =  []
for i in xrange(len(l_fit)):
    gen.append(i);

plt.style.use('grayscale') # seaborn-bright
plt.plot(gen, l_fit, label="largest fitness")
plt.plot(gen, a_fit, label="average fitness")
plt.ylabel('fitness')
plt.xlabel('generation')
plt.legend( loc='upper left', numpoints = 1 )
plt.grid(True)
plt.savefig("fitness_plot.png")
plt.show()
