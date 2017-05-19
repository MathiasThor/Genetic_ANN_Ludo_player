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

plt.axis([0, 1450, 9, 80])
plt.rcParams.update({'font.size': 20})
plt.gcf().subplots_adjust(bottom=0.12)
plt.style.use('seaborn-bright') # seaborn-bright
plt.plot(gen, l_fit, label="Largest fitness", linewidth=3)
plt.plot(gen, a_fit, label="Average fitness", linewidth=3)
plt.title('75% Crossover 30% Mutation')
plt.ylabel('Fitness')
plt.xlabel('Generation')
plt.legend( loc='lower right', numpoints = 1 )
plt.grid(True)
plt.savefig("fitness_plot.png")
plt.show()
