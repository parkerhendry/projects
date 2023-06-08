//set.js
//Parker Hendry
//09-02-22
//This program will perform set operations on two sets and return the union, intersection, or difference

//Function to compute set union
function setUnion(set1, set2)
{
	//create array to hold union
	var array = [];

	//add each element in set1 to array
	for (var i = 0; i < set1.length; i++)
	{	
		
		array.push(set1[i]);
	}	
	
	//add each element in set2 to array
	for (var i = 0; i < set2.length; i++)
	{
		array.push(set2[i]); 
	}
	
	
	//create temporary array to hold values
	var temp = [];
	
	var j = 0
	
	//Iterate through array and check for duplicates
	for (var i = 0; i < array.length; i++)
	{
		//if not a duplicate, add to temp
		if (array[i] != array[i + 1])
		{
			temp[j++] = array[i];
		}
	}
	
	//move temp back to array
	array = temp;
	
	//return set union
	return array;
}


//function to find set intersection
function setIntersection(set1, set2)
{

	//create array to hold intersection
	var array = [];


	//iterate through both sets
	//if two elements in each set are equal, add to array
	for (var i = 0; i < set1.length; i++)
	{
		for (var j = 0; j < set2.length; j++)
		{
			if (set1[i] == set2[j])
			{
				array.push(set1[i]);	
			}
		}
	}
	
	//return intersection
	return array;
}



//function to find set difference
/*THE CODE WITHIN THIS FUNCTION I OBTAINED FROM A STACKOVERFLOW POST*/
function setDifference(set1, set2)
{

	//for each element in set1, if it is not in set2, add to diff
	var diff = set1.filter(x => !set2.includes(x));
	
	//return set difference
	return diff;
}





	/*Decision structures for input validation*/
	if (process.argv[2] == "-h" || process.argv[2] == "--help")
	{
		//if help switch is used
		
	
		console.log("This app is used in the following way: ");
		console.log("set.js [SET OPERATION] [SET1] [SET2]");
		console.log("This only works with integer sets.");
	}
	else if (process.argv[2] == undefined)													
	{
	
		//if no switch is used
	
	
		console.log("Please enter a switch. Try -h or --help for help.");
		return -1;
	}
	else if (process.argv[3] == undefined || process.argv[4] == undefined)
	{
	
		//if no sets in arguments	
		
	
		console.log("Please enter two sets after the switch. Try -h or --help for help.");
		return -1;
	}
	else if (process.argv[5] != undefined)
	{
		//if too many sets
	
		console.log("Please enter only 3 arguments. The switch followed by two sets. Try -h or --help for help.");
		return -1;
	}
	else
	{	
		//variable to hold switch
		var arg = process.argv[2];
		
		//variable to hold set1
		var foo = process.argv[3];
		
		//variable to hold set2
		var bar = process.argv[4];

		//Remove the , that separates the numbers in the input
		var set1 = foo.split(',');
		var set2 = bar.split(',');
		
		//Remove the '' around the set elements
		set1 = set1.map(Number);
		set2 = set2.map(Number);
		
		//if any element is not an integer, return
		for (var i = 0; i < set1.length; i++)
		{
			if (!(Number.isInteger(set1[i])))
			{
				console.log("Please enter sets with integer values only. Try -h or --help for help.")
				return -1;
			}
		}
		
		//if any element is not an integer, return
		for (var i = 0; i < set2.length; i++)
		{
			if (!(Number.isInteger(set2[i])))
			{
				console.log("Please enter sets with integer values only. Try -h or --help for help.")
				return -1;
			}
		}
		
		//Call functions for set operations
		//Then print return value of each function
		if (arg == "-u" || arg == "--union")
		{
			var unionResult = setUnion(set1, set2);
			console.log(unionResult);
		}
		else if (arg == "-i" || arg == "--intersection")
		{
			var interResult = setIntersection(set1, set2);
			console.log(interResult);
		}
		else if (arg == "-d" || arg == "--difference")
		{
			var diffResult = setDifference(set1, set2);
			console.log(diffResult);
		}
	}


