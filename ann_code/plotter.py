import matplotlib.pyplot as plt
import numpy as np

# Create random data with numpy
import numpy as np

with open("tmp.txt") as f:
    error = f.readlines()

epokes =  []
for i in xrange(len(error)):
    epokes.append(i);

# plt.axis([0, 3000, 0, 0.3])
plt.rcParams.update({'font.size': 24})
plt.gcf().subplots_adjust(bottom=0.13)
plt.gcf().subplots_adjust(left=0.15)
plt.style.use('seaborn-bright') # seaborn-bright
plt.plot(epokes, error, linewidth=3)
plt.ylabel('Error')
plt.xlabel('Epoch')
#plt.title('Error')
plt.grid(True)
plt.savefig("test.png")
plt.show()
