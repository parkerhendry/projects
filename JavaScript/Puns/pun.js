//pun.js
//Parker Hendry
//10-14-22
//This program will print puns from the puns.dev module
//The puns will be printed in different colors using the chalk module


//load chalk module
const chalk = require("chalk");

//load puns.dev module
const puns = require("puns.dev");

//Function for random puns
function punRand()
{
	//Put random pun object in foo
	let foo = puns.random();

	//Print pun and punchline in magenta and cyan
	console.log(chalk.magentaBright(foo.pun));
	console.log(chalk.cyanBright(foo.punchline));	
}

function punID(id)
{

	//Get pun object at ID
	//Print pun and punchline in magenta and cyan
	console.log(chalk.magentaBright(puns.get(id).pun));
	console.log(chalk.cyanBright(puns.get(id).punchline));		
}

function punSearch(query)
{
	//Search for puns with specific keyword
	//Put all pun objects with that keyword into foo array
	let foo = puns.search(query);

	//Print all elements of foo array
	for (let i = 0; i < foo.length; i++)
	{
		console.log(chalk.magentaBright(foo[i].pun));
		console.log(chalk.cyanBright(foo[i].punchline));
	}		
}


	
	if (process.argv[2] == "-h" || process.argv[2] == "--help")
	{
	
		//If help switch, print help and exit
	
		console.log("This app is used in the following way: ");
		console.log("pun.js [OPTIONS]");
		console.log("Switches are optional.");
		return -1;
	}
	else if (process.argv[2] == undefined)
	{
		//If no command line arguments, call random function
		
		punRand();
	}
	else if (process.argv[2] == "-id" || process.argv[2] == "--id")
	{
		
		//If ID switch, call ID function
	
		//Input validation
		if (process.argv[3] == undefined)
		{
			console.log("Please enter an ID to search for after the switch.");
			return -1;
		}
	
		let id = process.argv[3];
		
		punID(id);	
		
	}
	else if (process.argv[2] == "-s" || process.argv[2] == "--search")
	{
		//If search switch, call search function
		
		//Input validation
		if (process.argv[3] == undefined)
		{
			console.log("Please enter a query to search for after the switch.");
			return -1;
		}
		
		let query = process.argv[3];
		
		punSearch(query);
				
	}	
