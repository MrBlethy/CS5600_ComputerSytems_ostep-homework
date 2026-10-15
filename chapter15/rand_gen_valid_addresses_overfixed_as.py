import matplotlib.pyplot as plt

# X asis : Bound Register Values
limits = [0, 100, 250, 500, 750,100]
# Y Axis : fractions organized per seeds 
seed1 = [0.1, 0.1, 0.1, 0.2, 0.3, 1.0]
seed2 = [0.1, 0.2, 0.2, 0.2, 0.3, 1.0]


#Graph setup
plt.plot(limits, seed1, marker='o', label= 'seed 1')
plt.plot(limits, seed2, marker='s', label= 'seed 2')

plt.xlabel("Bounds Registers")
plt.ylabel("fraction of Valid Adresses")
plt.title("Module 5 Homework question 5 Graph")

plt.legend()
plt.grid(True)
plt.show()
