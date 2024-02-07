import sys, re, math, random

NUMPY_AVAILABLE=True
try:
  import numpy as np
except:
  print("WARNING:  You do not appear to have the numpy package for python installed.  Some functionality is not available.")
  NUMPY_AVAILABLE=False


class Dataset:
  """
  This is a very simple class to pickup a variety of types of datasets
  to be used for classification (supervised learning).  It can read
  categorical, ordinal (integer), or numberic (real) data.  Attributes
  are named, and one attribute must be called 'class' if the user
  wishes to use it with any of the classifiers descending from
  BaseClassifier.  In such cases, the dataset will *create* a new
  attribute for each instance called 'assigned-class'.  The first is
  the true concept class value as given in the data file, while the
  second is there to facilitate the assignment of a class value by
  some classifier.

  The dataset will read a data file to populate its values.  The data
  file will have to obey a specific format.  There are two types of
  lines within the data file:

    1) Lines specifying attribute information
    2) Lines specifying instance data

  In the first case, lines must be in the form:
    <attribute-name> : <attribute-type> : <attribute-values>

  The attribute name can be any string that does not contain a ':'
  character, but they must be unique for each attribute.  Attribute
  type can be 'cat', 'ord', or 'num' for categorical, ordinal, or
  numeric data, respectively.  In the case of categorical data, the
  attribute values specify all the allowable values in that category.
  In the case of ordinal or numeric data, the Dataset file reader will
  establish a range based on the minimal and maximal values in the
  list provided.

  In the second case, lines must be in the form:
    <value-1>, <value-2>, ..., <value-n>

  There must be the same number of values in the line as their are
  attribute lines in the file.  The position of the lines does not
  matter, but the order does.  That is, The code will assume that the
  values are ordered in the same order as attributes are defined in
  the file.  The instance data is stored in the data set as a list of
  dictionaries.  Each instance is a dictionary keyed by attribute
  name.

  Here is an example:
    A:cat:a1,a2,a3
    B:ord:1,10
    C:num:3.4,9.6
    class:cat:-1,+1

    a1,  4,  6.1,  +1
    a2,  2,  3.55, -1
    a1,  2,  9.1,  +1
    a3,  3,  5.0,  -1
    a2,  10,  4.0, +1
  """

  # Categorical, ordinal (integer), and numeric (real) data types are permitted
  ATTRIBUTE_TYPES = set(["cat","ord","num"])

  def __init__(self,filename):
    """
    The Dataset constructor takes the provided file and populates its internal
    structures.  Data should be accessible after instantiation.
    """
    # Read the file into an internal buffer.  For convenience, remember the filename
    self.filename = filename
    # Initialize data structures ... not too python-y, but old habits die hard and
    # initialization is never a bad idea.
    self.revAttributeLU = {}  # To reverse lookup the attribute by index number
    self.attributes = {}      # The actual attribute metadata
    self.instances  = []      # The instance data
    self.classification = False

    if (not self.filename == None):
      f = open(self.filename,"r")
      fileLines = f.readlines()
      f.close()

      # Populate the metadata regarding the dataset from the file
      self.__populateAttributes(fileLines)

      # If there's not an attribute called 'class', the dataset cannot be used for classification
      self.classification = True
      if not "class" in self.attributes:
        self.classification = False

      # Populate the instance level data regarding the dataset from the file
      self.__populateInstances(fileLines)


  def __parseAttributeLine(self, attributeName, attributeType, attributeValues, idx):
    """
    This routine takes as input the name, type, and values from the
    line in the file, as well as the index order of the attribute in
    the file.  It cleans up and checks validity then create an
    attribute metadata record and store it in the attributes
    dictionary keyed by attribute name.  It also stores the index
    value in a reverse lookup dictionary, keyed by index and holding
    the attribute name.  This allows the dataset to easily keep the ordering
    consistent without painful linear lookups.
    """
    cleanedName = attributeName.strip()
    cleanedType = attributeType.strip().lower()[0:3]
    cleanedValList = []

    # Check attribute type specification to make sure it makes sense
    if not cleanedType in Dataset.ATTRIBUTE_TYPES:
      raise Exception("Specified type for attribute " + str(cleanedName) + " is illegal.  Please specify cat, ord, or num")

    if (cleanedType == "cat"):
      for val in attributeValues.split(','):
        cleanedValList.append(val.strip())

    elif cleanedType == "ord":
      x = []
      commaSplit = attributeValues.split(',')
      elipsisSplit = attributeValues.split('..')

      # If there are multiple values separated by commas, then
      # treat like a discrete set of ordinal values
      if (len(commaSplit) > 1):
        for val in commaSplit:
           x.append(int(val))
        cleanedValList = x[:]

      # If there are two values separated by elipsis,
      # then treat like a range of ordinal values
      elif (len(elipsisSplit) == 2):
        for val in elipsisSplit:
           x.append(int(val))
        cleanedValList = range(min(x), max(x)+1)

      else:
        raise Exception("Program could not understand ordinal attribute value specification for attribute " + str(cleanedName) + ".")

    elif cleanedType == "num":
      x = []
      for val in attributeValues.split(','):
         x.append(float(val))
      cleanedValList = [min(x), max(x)]

    # Create the attribute metadata tuple and store it in the attributes dictionary
    self.attributes[cleanedName] = (cleanedType,set(cleanedValList),idx)

    # Also store the index and name in the reverse lookup list
    self.revAttributeLU[idx] = cleanedName



  def __populateAttributes(self, fileLines):
    """
    Build the attributes metadata dictionary by parsing all lines in the file that
    have three fields separated by a ':'
    """
    idx = 0
    for line in fileLines:
      if (line.strip() == "") or (line.strip().startswith("#")):
        continue

      items = line.strip().split(":")
      if (len(items) == 3):
        self.__parseAttributeLine(items[0],items[1],items[2],idx)
        idx += 1


  def getAttributeType(self, attributeName):
    """
    Return the type of the attribute with the specified name.  Return 'None' if
    there is no such attribute.
    """
    if not attributeName in self.attributes:
        return None
    return self.attributes[attributeName][0]


  def getAttributeValues(self, attributeName):
    """
    Return the values or value range of the attribute with the specified name.  Return 'None' if
    there is no such attribute.
    """
    if not attributeName in self.attributes:
        return None
    return self.attributes[attributeName][1]


  def __parseInstanceColumn(self,idx,item):
    """
    This method considers a specific 'cell' in the data table, if you will.  It takes a particular
    value from a particular column (specified by idx), performs limited data type and value validity
    checks based on what is stored in the metadata, then sends the cleaned up value back to the
    instance line parser.
    """
    attributeName = self.revAttributeLU[idx]
    attribute = self.attributes[attributeName]
    attributeType = attribute[0]
    attributeValues = attribute[1]

    retVal = None

    # If it is categorical data, the value should be one of the categories for this
    # column's attribute.
    if (attributeType == "cat"):
      if not item.strip() in attributeValues:
        raise Exception("Categorical instance item " + str(item.strip()) + " is not in value set " + str(attributeValues))
      retVal = item.strip()

    # If it is ordinal data, then the value should be an integer and in the integer set
    # specified for this column's attribute.
    elif (attributeType == "ord"):
      val = int(item)
      if not val in attributeValues:
        raise Exception("Ordinal instance item " + str(val) + " is not in value set " + str(attributeValues))
      retVal = val

    # If it is numeric, then the value should be a floating point value and in the range
    # specified for this column's attribute.
    elif (attributeType == "num"):
      val = float(item)
      if (val < min(attributeValues)) or (val > max(attributeValues)):
        raise Exception("Numeric instance item " + str(val) + " is not in range " + str(attributeValues))
      retVal = val

    return retVal, attributeName


  def __parseInstanceLine(self,record):
    """
    Take the line from the data file that has already been split into fields based on
    a ',' delimeter, parse each item for each attribute / column, then create a dictionary for
    the instance keyed by attribute name for each column.  Add the instance dictionary to the
    self.instances list.
    """
    # There must be the same number of items in the instance record from the
    # file as there are attributes in the metadata structure.
    n = len(record)
    if (n < len(self.attributes)):
      raise Exception('Expecting ' + str(len(self.attributes)) + ' attributes, but found ' + str(n) + ' in line: ' + ','.join(record))

    instance = {}
    for idx in range(n):
      val, name = self.__parseInstanceColumn(idx,record[idx])
      instance[name] = val

    # In addition to the specified columns, add a new column for use by classifiers
    # in assigning a new class to the record.
    if self.classification:
      instance['assigned-class'] = None

    self.instances.append(instance)


  def __populateInstances(self, fileLines):
    """
    Spin through the whole file, and parse every line that has no ':' delimeter in it.
    Empty lines and lines beginning with '#' are ignored.  The latter allows the data
    file to be commented, if desired.
    """
    for line in fileLines:
      if (line.strip() == "") or (line.strip().startswith("#")):
        continue

      items = line.strip().split(":")
      if (len(items) == 1):
        self.__parseInstanceLine(line.strip().split(','))


  def printDataset(self, colWidth=10, otherInstances=None):
    """
    This is a simple convenience method, mainly for debugging, that prints out the data
    file in a tabular format.
    """
    # Print the header for the table.
    colNames = []
    for idx in range(len(self.revAttributeLU)):
      attributeName = self.revAttributeLU[idx]
      colNames.append(attributeName)
      print(attributeName[0:(colWidth-1)].ljust(colWidth), end='')
    if self.classification:
      print(" | Assigned class")
      print("-".ljust((colWidth+1)*(len(self.attributes)+2),'-'))
    else:
      print()

    # Print all instances in the dataset, unless given a different
    # set of instances (e.g., a subset) ... then print those.
    instanceList = self.instances
    if not otherInstances==None:
      instanceList = otherInstances

    # Print each instance line.
    idx = 0
    for instance in instanceList:
      for name in colNames:
        valStr = str( instance[name] )
        print(valStr[0:(colWidth-1)].ljust(colWidth), end='')
      if self.classification:
        print(" | " + str(instance['assigned-class']))
      else:
        print()
      idx += 1
    print()


  def getInstanceValue(self,attributeName,idx):
    """
    Return the value of a particular instance at some column.  The instance is specified
    using its index in the self.instances list and the column is specified using its
    attribute name.
    """
    if (idx < 0) or (idx >= len(self.instances)):
      raise Exception("Requesting instance " + str(idx) + ", but there are only " + str(len(self.instances)) + " instances in the dataset.")

    if not attributeName in self.instances[idx]:
      raise Exception("Requested attribute " + str(attributeName) + " for instance " + str(idx) + "; however, no such attribute exists.")

    return self.instances[idx][attributeName.strip()]


  def getInstanceSpaceDim(self, excludeList=['class','assigned-class']):
    """
    Return the dimensionality of the instance space.  Do not count attribute columns whose
    name are in the excludeList.  By default, this list includes 'class' and 'assigned-class'.
    """
    dim = len(self.attributes.keys())

    for exclStr in excludeList:
      if exclStr in self.attributes:
        dim -= 1

    return dim


  def assignClassToInstance(self,idx,classVal):
    """
    Assign a class value to a specified instance.
    """
    if (idx < 0) or (idx >= len(self.instances)):
      raise Exception("Requesting instance " + str(idx) + ", but there are only " + str(len(self.instances)) + " instances in the dataset.")

    if not self.classification:
      raise Exception("Attempting to assign class value to instance, but this is not a classification dataset")

    attribute = self.attributes['class']

    if not classVal in attribute[1]:
      raise Exception("There is no class value " + str(classVal) + " in the class attribute for this dataset")

    self.instances[idx]['assigned-class'] = classVal


  def matchInstance(self, instance, conditDict):
    """
    Return True if the instance matches all conditions in the condition dictionary
    and False otherwise.  See selectSubset() for more information about how these
    conditions are treated.
    """
    doInclude = True

    for variable in conditDict:
      val = instance[variable]
      if not val == conditDict[variable]:
        doInclude = False
        break

    return doInclude


  def instanceToVector(self, instance, bias=None, excludeList=['class','assigned=class']):
    """
    In many cases, it will be useful to have the instance level data in a numpy
    vector format.  This is a convenience method to convert all numeric and ordinal
    values in the instance to such a datatype.  Categorical columns are ignored
    entirely.
    """
    x = []
    if bias:
      x.append(bias)

    for variable in instance:
      try:
        if (variable in excludeList):
          continue
        val = float(instance[variable])
        x.append(val)
      except:
        pass

    safe_return_val = x
    if (NUMPY_AVAILABLE):
      safe_return_val = np.array(x)

    return safe_return_val


  def instanceToVectorByIdx(self, idx, bias=None):
    """
    In many cases, it will be useful to have the instance level data in a numpy
    vector format.  This is a convenience method to convert all numeric and ordinal
    values in the instance to such a datatype.  Categorical columns are ignored
    entirely.
    """
    return self.instanceToVector(self.instances[idx],bias)


  def strToType(self, attributeName, valueString):
    """
    A simple and limited method to convert a string value to the proper type for the
    dataset given the attribute.
    """
    typeName = self.getAttributeType(attributeName.strip()).lower()

    if typeName == 'num':
      return float(valueString)
    elif typeName == 'ord':
      return int(valueString)
    else:
      return valueString


  def selectSubset(self,conditDict):
    """
    This method takes a dictionary specifying attributes and their values and returns a list of instances
    for which all these hold true.  That is, it is a select using an 'AND' relationship amongst the
    conditions.  For example using the condition dictionary {'A':'a1', 'B':3} would find all instances
    in the data set in which the instance's 'A' attribute value is 'a1' *AND* in which it's 'B' attribute
    value is 3.  I.e., return the subset of instances for which A=a1 & B=3.
    """
    subset = []

    for instance in self.instances:
      if self.matchInstance(instance,conditDict):
        subset.append(instance.copy())

    return subset


  def divideDataset(self,prob):
    """
    Probabilistically separate all instances into one of two separate datasets.  Each line
    is assigned to a dataset independently at random using the specified probability.
    """
    ds1 = Dataset(self.filename)
    ds1.instances = []

    ds2 = Dataset(self.filename)
    ds2.instances = []

    for idx in range(len(self.instances)):
      if (random.random() < prob):
        ds1.instances.append(self.instances[idx])
      else:
        ds2.instances.append(self.instances[idx])

    return ds1, ds2


  def saveDataset(self,outfilename):
    """
    Write out the current dataset into the specified file in the correct format for
    reading in by the Dataset object at some other time.
    """
    outlines = []
    outlines.append("# Attribute Definitions\n")
    for attributeName in self.attributes:
      (attributeType, attributeValues, idx) = self.attributes[attributeName]

      lineStr = str(attributeName) + ":" + str(attributeType) + ":"
      for val in attributeValues:
        lineStr += str(val) + ","
      lineStr = lineStr[:-1] + "\n"
      outlines.append(lineStr)

    outlines.append("\n")

    outlines.append("# Instance Data\n")
    for instance in self.instances:
      lineStr = ""
      for attributeName in self.attributes:
        val = instance[attributeName]
        lineStr += str(val) + ","
      lineStr = lineStr[:-1] + "\n"
      outlines.append(lineStr)

    f = open(outfilename,"w")
    f.writelines(outlines)
    f.close()


def loadDatasetsFromCMDLine(cmdlineParams):
  """
  This is a utility function to facilitate picking up training set and test set
  filenames from the commandline, building the respective datasets, then returning them.
  """
  trainingFilename = cmdlineParams['training']
  testingFilename = cmdlineParams['testing']

  trainingSet = None
  testingSet = None

  print

  if (not trainingFilename == None):
    print("Training data from: ", trainingFilename)
    trainingSet = Dataset(trainingFilename)

  if (not testingFilename == None):
    print("Testing data from:  ", testingFilename)
    testingSet = Dataset(testingFilename)

  return trainingSet, testingSet


def unitTest():
  """
  This is a very simple unit test function for the Dataset class.  By default, it assumes there is a
  file called 'mushroom-training.data' in the current directory, though you can specify a different 
  file on the command line.
  """
  filename = "mushroom-training.data"
  selectionCriteria = {}

  # Get the file name from the first command line option
  if (len(sys.argv) > 1):
    filename = sys.argv[1]

  # Load the data set
  ds = Dataset(filename)

  # Print a list of the attribute names and values in the data set
  print("Attributes in data set:")
  for attr in ds.attributes:
    print(attr, ds.attributes[attr][0], ds.attributes[attr][1])
  print()

  # If selection criteria are given at the command-line, select and print only those instances
  # For instance, try running one of the following:
  #   python3 dataset.py mushroom-training.data "class=p"
  #   python3 dataset.py mushroom-training.data "class=p"
  if (len(sys.argv) > 2):
    selectionCriterialStrings = sys.argv[2].strip().split(',')
    for criteriaStr in selectionCriterialStrings:
      eqnComponents = criteriaStr.strip().split('=')
      if (len(eqnComponents) == 2):
        selectionCriteria[eqnComponents[0]] = ds.strToType(eqnComponents[0],eqnComponents[1])

    subsetOfInstances = ds.selectSubset(selectionCriteria)
    ds.printDataset(10,subsetOfInstances)
    print("Found", len(subsetOfInstances), "out of", len(ds.instances), "instances matching criteria", selectionCriteria)
    print()

  else:
    ds.printDataset()


if __name__ == '__main__':
  unitTest()
  #generateSpirals()
