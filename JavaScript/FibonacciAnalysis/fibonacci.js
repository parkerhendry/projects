//fibonacci.js
//Parker Hendry
//09-22-22
//This program will print each fibonacci number through n either iteratively or recursively
//Both methods will be timed and compared

//Closure function
function MyTimer()
{
	//private variables
	let startTime, lastElapsed, currentTime;

	//Function used to start timer
	function start()
	{
		//get current time and put it in startime
		startTime = new Date();
		
		//update lastelapsed
		lastElapsed = startTime;
		
	}
	
	//Function used to get elapsed time
	function elapsed()
	{
		
		//get current time and put it in currenttime
		currentTime = new Date();
		
		//put previous lastelapsed into temp
		let temp = lastElapsed;
		
		//update last elapsed
		lastElapsed = currentTime;
		
		//return time since last elapsed
		return (currentTime - temp);
	
	}
	
	//Function used to get total time since start
	function getTime()
	{
		//get current time and put it in currenttime
		currentTime = new Date();
		
		//return time since start
		return (currentTime - startTime);	
	}

	//Return object literal
	return {start, elapsed, getTime};
}

//Function to find n'th fibonacci number recursively
function fibonacci2n(n)
{
	if (n <= 1)
	{
		return n;
	}
	else
	{
		return fibonacci2n(n - 1) + fibonacci2n(n - 2);	
	}
}

//Function to find n'th fibonacci number iteratively
function fibonaccin(n)
{
	//Create timer
	let appTimer1 = MyTimer();
	
	//start timer
	appTimer1.start();

	let num1 = 1;
	let num2 = 1;
	let temp = 0;
	
	console.log("Fibonacci SequenceNum,Time Elapsed,Fibonacci Number");
	
	for (let ct = 1; ct <= n; ct++)
	{
		temp = num1;
		num1 = num1 + num2;
		num2 = temp;
		
		
		//Print results
		console.log(ct + ',' + appTimer1.elapsed() + ',' + num2);
		//console.log(typeof ',');
		
		
	}
	
	//Print total time
	console.log("Total Time: ", appTimer1.getTime());
	
	return num2;
}


/*Decision structures for input validation*/
if (process.argv[2] == '-h' || process.argv[2] == "--help")
{
	console.log("This app is used in the following way: ");
	console.log("node fibonacci.js [METHOD] [NUMBER]");
	return -1;
}
else if (process.argv[2] == undefined)
{
	console.log("Please enter a switch for either recursive or iterative method followed by a number.");
	return -1;
}
else if (process.argv[3] == undefined)
{
	console.log("Please enter a switch for either recursive or iterative method followed by a number.");
}
else if (process.argv[4] != undefined)
{
	console.log("Too many arguments.");
	return -1;
}
else if (process.argv[3] < 1)
{
	console.log("Number must be 1 or greater.");
}
else 
{

	//Get command line argument for n'th fibonacci number
	let n = process.argv[3];

	if (process.argv[2] == '-r' || process.argv[2] == "--recursive")
	{
		//Create timer
		let appTimer2 = MyTimer();
		
		//Start timer
		appTimer2.start();
		
		console.log("Fibonacci SequenceNum,Time Elapsed,Fibonacci Number");
	
		//Call recursive fibonacci function for each number through n
		//Then print results
		for (let i = 0; i < n; i++)
		{
			console.log((i + 1) + ',' + appTimer2.elapsed() + ',' + fibonacci2n(i + 2));		
		}
	
		//Print total time
		console.log("Total Time: ", appTimer2.getTime());
	}
	else if (process.argv[2] == '-i' || process.argv[2] == "--iterative")
	{
		//Call iterative fibonacci function
		fibonaccin(n);
	}		
}






















