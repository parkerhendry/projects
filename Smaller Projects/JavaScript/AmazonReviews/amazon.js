//amazon.js
//Parker Hendry
//10-31-22
/*
This program will count the number of helpful votes and unhelpful votes from a large .tsv file of Amazon reviews and compute the average of helpful votes and unhelpful votes.
*/


//Load modules
import { createRequire } from 'module';
const require = createRequire(import.meta.url);
const Big = require('big.js');
import { tsvParseRows } from 'd3-dsv';

//Create stream from fs module
var rl = require('readline').createInterface
({
    input : require('fs').createReadStream('amazon_reviews_us_Mobile_Apps_v1_00.tsv')
});

//Create big values to hold total helpful votes and total unhelpful votes
let helpTotal = new Big(0.0);
let unhelpTotal = new Big(0.0);

//Variable to hold number of lines
var j = 0;

//Listen for line event, store line in str
rl.on('line', function (str) 
{
		//Put line without tabs into line[i]
		let line = tsvParseRows(str);
	
		//If after first line
		if (j > 0)
		{
		
			
			//Create big values to hold helpful and unhelpful votes
			let helpVotes = new Big(parseInt(line[0][8]));
			let unhelpVotes = new Big(parseInt(line[0][9]) - parseInt(line[0][8]));
			
			//Add helpful votes from line to helpTotal
			helpTotal = helpTotal.plus(helpVotes);
			
			//Add unhelpful votes from line to unhelpTotal
			unhelpTotal = unhelpTotal.plus(unhelpVotes);
			
		}
		
		//Increment number of lines
		j++;	
});

//Listen for close event
rl.on('close', function ()
{

	let numLines = new Big(j);

	//Divide total by the number of lines
	let helpAvg = helpTotal.div(numLines - 1);
	let unhelpAvg = unhelpTotal.div(numLines - 1);

	//Print results
	console.log("Avg Helpful: ", helpAvg.toFixed(19));
	console.log("Avg Unhelpful: ", unhelpAvg.toFixed(20));


});
