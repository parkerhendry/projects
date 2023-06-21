//Local module file

//load smurf.json
const phrases = require("./smurf.json");

//Create function to be exported
exports.translation = function (phrase)
{

       
	//Turn phrase to all lowercase
	phrase = phrase.toLowerCase();
	
	//Remove apostrophe's if they exist
	phrase = phrase.replace('\'', '');	
	
	//Iterate through phrases, replacing (within the phrase) keys with their values
	for (let [key, value] of Object.entries(phrases))
	{
		phrase = phrase.replace(key, value);
	}
	
	return phrase;
	
}
