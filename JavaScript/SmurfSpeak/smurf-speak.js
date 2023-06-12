//smurf-speak.js
//Parker Hendry
//10-14-22
//This program will accept a limited number of phrases that can be translated into "smurf speak"


//load translate.js module file
const trans = require("./translate");


if (process.argv[2] == undefined)
{
	//If no phrase is entered, print help and exit	
	
	console.log("Please enter a phrase after smurf-speak.js");
	console.log("node smurf-speak.js [USERTEXT]");
	return -1;
}
else
{
	userText = process.argv[2];
	
	//Call translation function from module file	
	let answer = trans.translation(userText);
	
	//Print answer
	console.log(answer);
}
