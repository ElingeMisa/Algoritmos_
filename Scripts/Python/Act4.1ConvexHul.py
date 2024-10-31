from scipy.spatial import ConvexHull
import numpy as np

# Import necessary libraries
import matplotlib.pyplot as plt

# Generate random points
points = np.random.rand(30, 2)

# Compute the convex hull
hull = ConvexHull(points)

# Plot the points
plt.plot(points[:, 0], points[:, 1], 'o')

# Plot the convex hull
for simplex in hull.simplices:
    plt.plot(points[simplex, 0], points[simplex, 1], 'k-')

plt.xlabel('X-axis')
plt.ylabel('Y-axis')
plt.title('Convex Hull')
plt.show()