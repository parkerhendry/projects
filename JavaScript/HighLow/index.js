//index.js
//Parker Hendry
//11-14-22
/*
This app uses Express and Twilio to create an SMS high-low game. Each time a text is sent to Twilio, Twilio makes an HTTP
POST request to the Express web server. This app then creates the appropriate response based on the request. 
*/


const express = require('express');
const bodyParser = require('body-parser');
const twilio = require('twilio');
let app = express();

const accountSid = "ACcbae46ae66f2a9b12fd3f56d92ba9e2a";
const authToken = "782cb8ce578f3dd7f2983bab2c9a404e";

const client = new twilio(accountSid, authToken);

const MessagingResponse = require('twilio').twiml.MessagingResponse;

app.use(bodyParser.urlencoded({extended:false}));

//Create array to hold user's random number and wins
let data = [];

app.post('/sms', function (request, response)
{

	const twiml = new MessagingResponse();
	
	response.writeHead(200, {'Content-Type' : 'text/xml'});
	
	//Get the contents of the text
	let body = request.body.Body.toLowerCase();
	
	//Get the user's identity
	let user = request.body.From;
	
	
	if (body == 'play')
	{
		//If the user enter's play, generate random number and begin the game
	
		data[user + " random"] = Math.floor(Math.random() * 10 + 1);
		
		twiml.message("Please guess a number from 1 to 10.")
	}
	else if (parseInt(body) < data[user + " random"])
	{
		twiml.message("Your guess is too low. Please try again.")
	}
	else if (parseInt(body) > data[user + " random"])
	{
		twiml.message("Your guess is too high. Please try again.");
	}
	else if (parseInt(body) == data[user + " random"])
	{
		//If user guesses correctly, increment their number of wins
	
		if (data[user + " wins"] == undefined)
		{
			data[user + " wins"] = 1;
		}
		else
		{
			data[user + " wins"]++;
		}
		
		twiml.message(`Correct! You have won ${data[user + " wins"]} games. If you would like to play again, enter \"play\"!`);
	}
	else
	{
		//Give user instructions if they enter something that is not related to the game
		
		twiml.message("Enter \"play\" to begin high-low game!");
		
	}
	
	response.end(twiml.toString());

});

app.listen(8000);
