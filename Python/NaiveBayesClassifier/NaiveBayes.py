'''
Naive Bayes Classifier By Parker Hendry
01/31/2024


*  Is the naive Bayes assumption valid and correct here? If not, does it matter here?

   - The Naive Bayes assumption seems to not be valid or correct here. Since this model has over 100 different attributes, it is likely that some are correlated. As the number of attributes grows, independence confidence is restricted. It does not matter here, as the accuracy is still very high. Naive Bayes still performs well even though the assumption is violated, as the data is most likely not heavily correlated. 
   
  
   
*  Did using an *m*-estimate help improve the classifier performance? Why or why not?

   - When using m = 0, the classifier is the most accurate. When increasing m, the accuracy decreases. This could be because of over-smoothing. Over-smoothing could create similar probabilities for different attributes, making it more difficult for the algorithm to classify instances. 
'''
import dataset

#Label instances
#(Pr(A|e)*Pr(B|e)*Pr(C|e)*...*Pr(e))
def labelInstances(foo):
	for instance in foo.instances:
		a = 1
		b = 1
		c = 1
		d = 1 
		for key in instance:
			if key == 'assigned-class':
				b *= ediblePr
				d *= poisonPr
				if b > d:
					instance[key] = 'e'
				elif b < d:
					instance[key] = 'p'
				continue
			if key == 'class':
				continue;
			a *= priorPr[key]['edible'][instance[key]]
			b *= postPr[key]['edible'][instance[key]] 
			c *= priorPr[key]['poison'][instance[key]]
			d *= postPr[key]['poison'][instance[key]]
	
#Test accuracy				
def testAccuracy(foo):
	total = 0
	for instance in foo.instances:
		if instance['class'] == instance['assigned-class']:
			total += 1	

	total /= len(foo.instances)

	total *= 100

	return total	
	
	
	
#Start of main
training = dataset.Dataset("mushroom-training.data")
testing = dataset.Dataset("mushroom-testing.data")

poison = {"class":"p"}
edible = {"class":"e"}

m = int(input("M estimate to be used: "))

#Find poison and edible probabilities
poisonPr = (len(training.selectSubset(poison)) + m) / (len(training.instances) + m)
ediblePr = (len(training.selectSubset(edible)) + m) / (len(training.instances) + m)

postPr = {

	'habitat': {
		'edible': {'d': 0, 'g': 0, 'm': 0, 'l': 0, 'p': 0, 'u': 0, 'w': 0},
		'poison': {'d': 0, 'g': 0, 'm': 0, 'l': 0, 'p': 0, 'u': 0, 'w': 0}
	},
	'cap-shape': {
		'edible': {'c': 0, 'b': 0, 'f': 0, 'k': 0, 's': 0, 'x': 0},
		'poison': {'c': 0, 'b': 0, 'f': 0, 'k': 0, 's': 0, 'x': 0}
	},
	'cap-color': {
		'edible': {'c': 0, 'b': 0, 'e': 0, 'g': 0, 'n': 0, 'p': 0, 'r': 0, 'u': 0, 'w': 0, 'y': 0},
		'poison': {'c': 0, 'b': 0, 'e': 0, 'g': 0, 'n': 0, 'p': 0, 'r': 0, 'u': 0, 'w': 0, 'y': 0}
	},
	'stalk-color-above-ring': {
		'edible': {'c': 0, 'b': 0, 'e': 0, 'g': 0, 'o': 0, 'n': 0, 'p': 0, 'w': 0, 'y': 0},
		'poison': {'c': 0, 'b': 0, 'e': 0, 'g': 0, 'o': 0, 'n': 0, 'p': 0, 'w': 0, 'y': 0}
	},
	'stalk-shape': {
		'edible': {'e': 0, 't':0},
		'poison': {'e': 0, 't':0}
	},
	'gill-attachment': {
		'edible': {'a': 0, 'n': 0, 'd': 0, 'f': 0},
		'poison': {'a': 0, 'n': 0, 'd': 0, 'f': 0}
	},
	'ring-number': {
		'edible': {'t': 0, 'o': 0, 'n': 0},
		'poison': {'t': 0, 'o': 0, 'n': 0}
	},
	'odor': {
		'edible': {'a': 0, 'c': 0, 'f': 0, 'm': 0, 'l': 0, 'n': 0, 'p': 0, 's': 0, 'y': 0},
		'poison': {'a': 0, 'c': 0, 'f': 0, 'm': 0, 'l': 0, 'n': 0, 'p': 0, 's': 0, 'y': 0}
	},
	'gill-size': {
		'edible': {'b': 0, 'n': 0},
		'poison': {'b': 0, 'n': 0}
	},
	'stalk-color-below-ring': {
		'edible': {'c': 0, 'b': 0, 'e': 0, 'g': 0, 'o': 0, 'n': 0, 'p': 0, 'w': 0, 'y': 0},
		'poison': {'c': 0, 'b': 0, 'e': 0, 'g': 0, 'o': 0, 'n': 0, 'p': 0, 'w': 0, 'y': 0}
	},
	'veil-color': {
		'edible': {'y': 0, 'w': 0, 'o': 0, 'n': 0},
		'poison': {'y': 0, 'w': 0, 'o': 0, 'n': 0}
	},
	'stalk-surface-below-ring': {
		'edible': {'y': 0, 'k': 0, 's': 0, 'f': 0},
		'poison': {'y': 0, 'k': 0, 's': 0, 'f': 0}
	},
	'gill-spacing': {
		'edible': {'c': 0, 'd': 0, 'w': 0},
		'poison': {'c': 0, 'd': 0, 'w': 0}
	},
	'spore-print-color': {
		'edible': {'b': 0, 'h': 0, 'k': 0, 'o': 0, 'n': 0, 'r': 0, 'u': 0, 'w': 0, 'y': 0},
		'poison': {'b': 0, 'h': 0, 'k': 0, 'o': 0, 'n': 0, 'r': 0, 'u': 0, 'w': 0, 'y': 0}
	},
	'gill-color': {
		'edible': {'b': 0, 'e': 0, 'g': 0, 'h': 0, 'k': 0, 'o': 0, 'n': 0, 'p': 0, 'r': 0, 'u': 0, 'w': 0, 'y': 0},
		'poison': {'b': 0, 'e': 0, 'g': 0, 'h': 0, 'k': 0, 'o': 0, 'n': 0, 'p': 0, 'r': 0, 'u': 0, 'w': 0, 'y': 0}
	},
	'population': {
		'edible': {'a': 0, 'c': 0, 'n': 0, 's': 0, 'v': 0, 'y': 0},
		'poison': {'a': 0, 'c': 0, 'n': 0, 's': 0, 'v': 0, 'y': 0}
	},
	'stalk-root': {
		'edible': {'c': 0, 'b': 0, 'e': 0, 'r': 0, 'u': 0, 'z': 0, '?': 0},
		'poison': {'c': 0, 'b': 0, 'e': 0, 'r': 0, 'u': 0, 'z': 0, '?': 0}
	},
	'ring-type': {
		'edible': {'c': 0, 'e': 0, 'f': 0, 'l': 0, 'n': 0, 'p': 0, 's': 0, 'z': 0},
		'poison': {'c': 0, 'e': 0, 'f': 0, 'l': 0, 'n': 0, 'p': 0, 's': 0, 'z': 0}
	},
	'bruises': {
		'edible': {'t': 0, 'f': 0},
		'poison': {'t': 0, 'f': 0}
	},
	'stalk-surface-above-ring': {
		'edible': {'y': 0, 'k': 0, 's': 0, 'f': 0},
		'poison': {'y': 0, 'k': 0, 's': 0, 'f': 0}
	},
	'veil-type': {
		'edible': {'p': 0, 'u': 0},
		'poison': {'p': 0, 'u': 0}
	},
	'cap-surface': {
		'edible': {'y': 0, 's': 0, 'g': 0, 'f': 0},
		'poison': {'y': 0, 's': 0, 'g': 0, 'f': 0}
	}
}

priorPr = {

	'habitat': {
		'edible': {'d': 0, 'g': 0, 'm': 0, 'l': 0, 'p': 0, 'u': 0, 'w': 0},
		'poison': {'d': 0, 'g': 0, 'm': 0, 'l': 0, 'p': 0, 'u': 0, 'w': 0}
	},
	'cap-shape': {
		'edible': {'c': 0, 'b': 0, 'f': 0, 'k': 0, 's': 0, 'x': 0},
		'poison': {'c': 0, 'b': 0, 'f': 0, 'k': 0, 's': 0, 'x': 0}
	},
	'cap-color': {
		'edible': {'c': 0, 'b': 0, 'e': 0, 'g': 0, 'n': 0, 'p': 0, 'r': 0, 'u': 0, 'w': 0, 'y': 0},
		'poison': {'c': 0, 'b': 0, 'e': 0, 'g': 0, 'n': 0, 'p': 0, 'r': 0, 'u': 0, 'w': 0, 'y': 0}
	},
	'stalk-color-above-ring': {
		'edible': {'c': 0, 'b': 0, 'e': 0, 'g': 0, 'o': 0, 'n': 0, 'p': 0, 'w': 0, 'y': 0},
		'poison': {'c': 0, 'b': 0, 'e': 0, 'g': 0, 'o': 0, 'n': 0, 'p': 0, 'w': 0, 'y': 0}
	},
	'stalk-shape': {
		'edible': {'e': 0, 't':0},
		'poison': {'e': 0, 't':0}
	},
	'gill-attachment': {
		'edible': {'a': 0, 'n': 0, 'd': 0, 'f': 0},
		'poison': {'a': 0, 'n': 0, 'd': 0, 'f': 0}
	},
	'ring-number': {
		'edible': {'t': 0, 'o': 0, 'n': 0},
		'poison': {'t': 0, 'o': 0, 'n': 0}
	},
	'odor': {
		'edible': {'a': 0, 'c': 0, 'f': 0, 'm': 0, 'l': 0, 'n': 0, 'p': 0, 's': 0, 'y': 0},
		'poison': {'a': 0, 'c': 0, 'f': 0, 'm': 0, 'l': 0, 'n': 0, 'p': 0, 's': 0, 'y': 0}
	},
	'gill-size': {
		'edible': {'b': 0, 'n': 0},
		'poison': {'b': 0, 'n': 0}
	},
	'stalk-color-below-ring': {
		'edible': {'c': 0, 'b': 0, 'e': 0, 'g': 0, 'o': 0, 'n': 0, 'p': 0, 'w': 0, 'y': 0},
		'poison': {'c': 0, 'b': 0, 'e': 0, 'g': 0, 'o': 0, 'n': 0, 'p': 0, 'w': 0, 'y': 0}
	},
	'veil-color': {
		'edible': {'y': 0, 'w': 0, 'o': 0, 'n': 0},
		'poison': {'y': 0, 'w': 0, 'o': 0, 'n': 0}
	},
	'stalk-surface-below-ring': {
		'edible': {'y': 0, 'k': 0, 's': 0, 'f': 0},
		'poison': {'y': 0, 'k': 0, 's': 0, 'f': 0}
	},
	'gill-spacing': {
		'edible': {'c': 0, 'd': 0, 'w': 0},
		'poison': {'c': 0, 'd': 0, 'w': 0}
	},
	'spore-print-color': {
		'edible': {'b': 0, 'h': 0, 'k': 0, 'o': 0, 'n': 0, 'r': 0, 'u': 0, 'w': 0, 'y': 0},
		'poison': {'b': 0, 'h': 0, 'k': 0, 'o': 0, 'n': 0, 'r': 0, 'u': 0, 'w': 0, 'y': 0}
	},
	'gill-color': {
		'edible': {'b': 0, 'e': 0, 'g': 0, 'h': 0, 'k': 0, 'o': 0, 'n': 0, 'p': 0, 'r': 0, 'u': 0, 'w': 0, 'y': 0},
		'poison': {'b': 0, 'e': 0, 'g': 0, 'h': 0, 'k': 0, 'o': 0, 'n': 0, 'p': 0, 'r': 0, 'u': 0, 'w': 0, 'y': 0}
	},
	'population': {
		'edible': {'a': 0, 'c': 0, 'n': 0, 's': 0, 'v': 0, 'y': 0},
		'poison': {'a': 0, 'c': 0, 'n': 0, 's': 0, 'v': 0, 'y': 0}
	},
	'stalk-root': {
		'edible': {'c': 0, 'b': 0, 'e': 0, 'r': 0, 'u': 0, 'z': 0, '?': 0},
		'poison': {'c': 0, 'b': 0, 'e': 0, 'r': 0, 'u': 0, 'z': 0, '?': 0}
	},
	'ring-type': {
		'edible': {'c': 0, 'e': 0, 'f': 0, 'l': 0, 'n': 0, 'p': 0, 's': 0, 'z': 0},
		'poison': {'c': 0, 'e': 0, 'f': 0, 'l': 0, 'n': 0, 'p': 0, 's': 0, 'z': 0}
	},
	'bruises': {
		'edible': {'t': 0, 'f': 0},
		'poison': {'t': 0, 'f': 0}
	},
	'stalk-surface-above-ring': {
		'edible': {'y': 0, 'k': 0, 's': 0, 'f': 0},
		'poison': {'y': 0, 'k': 0, 's': 0, 'f': 0}
	},
	'veil-type': {
		'edible': {'p': 0, 'u': 0},
		'poison': {'p': 0, 'u': 0}
	},
	'cap-surface': {
		'edible': {'y': 0, 's': 0, 'g': 0, 'f': 0},
		'poison': {'y': 0, 's': 0, 'g': 0, 'f': 0}
	}
}

#Find totals for each attribute
for instance in training.instances:
	for key in instance: 
		if key == 'assigned-class' or key == 'class':
			continue
		if instance['class'] == 'e':
			priorPr[key]['edible'][instance[key]] += 1
			postPr[key]['edible'][instance[key]] += 1
		elif instance['class'] == 'p':
			priorPr[key]['poison'][instance[key]] += 1
			postPr[key]['poison'][instance[key]] += 1

#Find prior and posterior probabilities	for each attribute
#(Nc + mp) / (n + m)	
for foo in priorPr:
	for baz in priorPr[foo]['edible']:
		priorPr[foo]['edible'][baz] = ((priorPr[foo]['edible'][baz] + m) / (len(training.instances) + m))
		postPr[foo]['edible'][baz] = ((postPr[foo]['edible'][baz] + m * priorPr[foo]['edible'][baz]) / (len(training.selectSubset(edible)) + m))
	for baz in priorPr[foo]['poison']:
		priorPr[foo]['poison'][baz] = ((priorPr[foo]['poison'][baz] + m) / (len(training.instances) + m))
		postPr[foo]['poison'][baz] = ((postPr[foo]['poison'][baz] + m * priorPr[foo]['poison'][baz]) / (len(training.selectSubset(poison)) + m))

#Label instances
#(Pr(A|e)*Pr(B|e)*Pr(C|e)*...*Pr(e))
labelInstances(training)
labelInstances(testing)

#Test accuracy		
trainResult = testAccuracy(training)
testResult = testAccuracy(testing)

print("Training Accuracy: ", trainResult, "%")
print("Testing Accuracy: ", testResult, "%")
		
	
					
