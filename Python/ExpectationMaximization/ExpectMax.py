# Parker Hendry
# 03-31-24

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import davies_bouldin_score

def ExpectMax(k, s, pr3):

	m, d = pr3.shape

	# Use random points for initial centroids
	rand = np.random.choice(range(m), k, replace=False)
	cent = pr3[rand]
	
	# Initialize empty structure to later hold assignments
	hypo = np.zeros(m)
	
	notDone = True
	
	while notDone:	
		
		# E STEP
		
		# Find distances
		# - Change pr3 from (m x d) to (m x k x d)
		# - Each point in pr3 is subtracted from EVERY centroid
		# - Sum x+y for each centroid, giving distances to each centroid
		#   for each data point
		distances = np.sum(np.square(pr3[:, np.newaxis, :] - cent), axis=2)
		
		# Calculate probabilities
		probs = np.exp(-1/(2*np.square(s)) * distances)		
		probs /= np.sum(probs, axis=1, keepdims=True)
		
		# Assign points to centroids, creating clusters
		new = np.argmax(probs, axis=1)
		
		
		# M STEP
		
		# Update centroids
		# (probs^T*pr3) / sum(probs(i,j))
		# - Transpose probs to get (k x m) * (m x d) resulting in (k x d)
		# - Divide by the sum of probs
		cent = np.dot(probs.T, pr3)
		summ = np.sum(probs.T, axis=1, keepdims=True)
		empty = summ == 0
		summ[empty] = 1
		cent /= summ		
		
		# Check for convergence
		if np.array_equal(hypo, new):
			notDone = False
		else:
			hypo = new
	
	#Compute davies bouldin	
	score = davies_bouldin_score(pr3, hypo)	
	print(f'Davies Bouldin Score for k = {k:.2f}, sigma = {s:.2f} : {score:.4f}')
	
	# Create scatterplot for visualization
	plt.figure(figsize=(12, 12))
	plt.scatter(pr3[:, 0], pr3[:, 1], c=hypo, cmap='plasma', s=100, alpha=0.5)
	plt.scatter(cent[:, 0], cent[:, 1], c='red', marker='X', s=200, label='Centroids')
	plt.title('Expectation Maximization', fontsize=40)
	plt.figtext(0.5, 0.01, f'Davies Bouldin Score for k = {k:.2f}, sigma = {s:.2f} : {score:.4f}', ha='center', fontsize=20)

	plt.legend()
	plt.show()

pr3 = np.array(pd.read_csv('pr3.data'))
			
k = int(input('Enter a value for k: '))
s = float(input('Enter a value for sigma: '))
ExpectMax(k, s, pr3)

