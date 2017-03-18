import matplotlib.pyplot as plt
import numpy as np

# Create random data with numpy
import numpy as np

with open("tmp.txt") as f:
    error = f.readlines()

epokes =  []
for i in xrange(len(error)):
    epokes.append(i);

plt.style.use('grayscale') # seaborn-bright	
plt.plot(epokes, error)
plt.ylabel('error')
plt.xlabel('epoch')
#plt.title('Error')
plt.grid(True)
plt.savefig("test.png")
plt.show()
