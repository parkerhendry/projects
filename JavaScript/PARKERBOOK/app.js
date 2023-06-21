var express = require('express');
var bodyParser = require('body-parser');
var session = require('express-session');
var mongoose = require('mongoose');
var fs = require('fs');
var path = require('path');

var app = express();

var usersDB = mongoose.connect('mongodb://localhost/users');

app.set('view engine', 'pug');
app.set('views', './views');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(session({secret: "Shh, its a secret!", resave: true, saveUninitialized: true}));
app.use(express.static('uploads'));

var loggedIn = 0;

var userSchema = mongoose.Schema(
{
	id: String,
	password: String,
	posts: [Object],
	friends: [String],
	profile: Object
});

var User = mongoose.model('User', userSchema);

var multer = require('multer');

var storage = multer.diskStorage(
{

	destination: (req, file, cb) => 
	{
		cb(null, 'uploads')
	},
	filename: (req, file, cb) => 
	{
		cb(null, file.fieldname + '-' + Date.now())
	}
});


var upload = multer({ storage: storage });


app.get('/', (req, res) => 
{	
	User.find().then((response) =>
	{
		var array = [];
	
		for (var i = 0; i < response.length; i++)
		{
			for (var j = 0; j < response[i].posts.length; j++)
			{
				array.push(response[i].posts[j]);	
			}
		}
		
		array.sort( (a, b) =>
		{
			return new Date(b.date) - new Date(a.date);
		});
	
		if (loggedIn == 0)
			res.render('home', {response: array});
		else
			res.render('home', {response: array, loggedIn: 'Yes'});				
	});	
});

app.get('/signup', (req, res) =>
{
	if (loggedIn == 0)
		res.render('signup', {message: 'Please create an account!'});
	else
		res.render('signup', {message: 'Please create an account!', loggedIn: 'Yes'});
});

app.post('/signup', (req, res) =>
{
	if (!req.body.id || !req.body.password)
	{
		res.send('Invalid details');
	}
	else
	{
		
		User.find({id: req.body.id}).then((response) =>
		{
			var foo = JSON.stringify(response);
		
			if (foo == '[]')
			{	
				var newUser = new User(
				{
					id: req.body.id,
					password: req.body.password
				});
			
				newUser.save().then(function ()
				{
					req.session.user = newUser;
				
					loggedIn = 1;
			
					res.redirect('/');
			
				}).catch(function (err)
				{
					res.send('Error');
			
				});			
			}
			else
			{
				res.render('signup', {message: 'User already exists'});				
			}
		

				
		});
		
	}	
});

app.get('/protected_page', (req, res) => 
{
	User.find().then((response) =>
	{
		if (loggedIn == 0)
			res.render('protected_page', {response: response});
		else
			res.render('protected_page', {response: response, loggedIn: 'Yes'});		
	});
});

app.post('/protected_page', (req, res) =>
{
	res.redirect('/logout');
});

app.get('/login', (req, res) =>
{
	if (loggedIn == 0)
		res.render('login', {message: 'Please log in!'});
	else
		res.render('login', {message: 'Please log in!', loggedIn: 'Yes'});
});

app.post('/login', (req, res) =>
{
	if (!req.body.id || !req.body.password)
	{
		res.render('login', {message: 'Please enter both id and password'});
	}
	else
	{	
		
		User.find({id: req.body.id, password: req.body.password}).then((response) =>
		{
			var foo = JSON.stringify(response);
		
			if (foo == '[]')
			{
				if (loggedIn == 0)
					res.render('login', {message: 'Invalid credentials!'});	
				else
					res.render('login', {message: 'Invalid credentials!', loggedIn: 'Yes'});			
			}			
			else
			{
				var newUser = {id: response[0].id, password: response[0].password, posts: response[0].posts};
				
				req.session.user = newUser;
				
				res.redirect('/');
				
				loggedIn = 1;				
			}
	
				
		});		
		
	}
});

app.get('/logout', (req, res) =>
{
	loggedIn = 0;

	req.session.destroy();

	res.redirect('/login');
});

app.get('/newpost', (req, res) =>
{

	if (loggedIn == 0)
	{
		res.render('login', {message: 'You must first login before you can post!'});
	}	
	else
	{
		res.render('newpost', {loggedIn: 'Yes'});
	}
});


app.post('/newpost', upload.single('image'), (req, res, next) =>
{
	var finish = {};
	
	User.find({id: req.session.user.id}).then((response) =>
	{
		finish = response;
		
		var datetime = new Date().toLocaleString();
		
		if (req.file != undefined)
		{
			var friend = {id: req.session.user.id, topic: req.body.topic, body: req.body.body, date: datetime, category: req.body.category, data: fs.readFileSync(path.join(__dirname + '/uploads/' + req.file.filename)), contentType: 'image/png', comments: []};
			
			friend.data = friend.data.toString('base64');
		}
		else
		{
			var friend = {id: req.session.user.id, topic: req.body.topic, body: req.body.body, date: datetime, category: req.body.category, comments: []};				
		}
			
		finish[0].posts.unshift(friend);
			
		User.findOneAndUpdate({id: req.session.user.id}, {posts: finish[0].posts}).then((response) =>
		{
			res.redirect('/');
		});
		
				
	});
	
	
	
});


app.get('/profile', (req, res) =>
{
	if (loggedIn == 0)
	{
		res.render('login', {message: 'You must first login before you can view your profile!'});
	}
	else
	{
	 	User.find({id: req.session.user.id}).then((response) =>
	 	{
	 		res.render('profile', {bar: response[0], loggedIn: 'Yes'});		
		});	
	}
	
});

app.get('/editprofile', (req, res) =>
{
	res.render('editprofile', {loggedIn: 'Yes'});
});

app.post('/editprofile', upload.single('image'), (req, res, next) =>
{
	User.find({id: req.session.user.id}).then((response) =>
	{
		
		var fred = {name: req.body.name, interest1: req.body.interest1, interest2: req.body.interest2, interest3: req.body.interest3, status: req.body.status, pic: fs.readFileSync(path.join(__dirname + '/uploads/' + req.file.filename)), contentType: 'image/png'};
		
		fred.pic = fred.pic.toString('base64');
		
		User.findOneAndUpdate({id: req.session.user.id}, {profile: fred}).then((response) =>
		{
			res.redirect('/profile');
		});
			
	});
});

app.get('/messages', (req, res) =>
{
	res.send('Page has not yet been made! -Parker');
});

app.post('/foopost', (req, res) =>
{		
	User.find({id: req.body.postID}).then((response) =>
	{
		var baz = response[0].posts;
		
		for (var i = 0; i < baz.length; i++)
		{
			if (baz[i].topic == req.body.postTopic && baz[i].date == req.body.postDate)
			{
				if (loggedIn == 0)
					res.render('viewpost', {post: baz[i]});
				else
					res.render('viewpost', {post: baz[i], loggedIn: 'Yes'});
				 
			}
		}
	});
});

app.post('/like', (req, res) =>
{
	if (loggedIn == 0)
	{
		res.render('login', {message: 'You must first login before you can like a post!'});
	}
	else
	{

	User.find({id: req.body.postID}).then((response) =>
	{
		var baz = response[0].posts;
		
		for (var i = 0; i < baz.length; i++)
		{
			if (baz[i].topic == req.body.postTopic && baz[i].date == req.body.postDate)
			{
				if (baz[i].likes == undefined)
					baz[i].likes = 1;
				else
					baz[i].likes++;
					
				User.findOneAndUpdate({id: req.body.postID}, {posts: baz}).then((response) =>
				{
					//stuff					
				});
				
				if (loggedIn == 0)
					res.render('viewpost', {post: baz[i]});
				else
					res.render('viewpost', {post: baz[i], loggedIn: 'Yes'});
				 
			}
		}
	});
	
	}		
});

app.post('/dislike', (req, res) =>
{
	if (loggedIn == 0)
	{
		res.render('login', {message: 'You must first login before you can dislike a post!'});	
	}
	else
	{

	User.find({id: req.body.postID}).then((response) =>
	{
		var baz = response[0].posts;
		
		for (var i = 0; i < baz.length; i++)
		{
			if (baz[i].topic == req.body.postTopic && baz[i].date == req.body.postDate)
			{
				if (baz[i].dislikes == undefined)
					baz[i].dislikes = 1;
				else
					baz[i].dislikes++;
					
				User.findOneAndUpdate({id: req.body.postID}, {posts: baz}).then((response) =>
				{
					//stuff					
				});
				
				if (loggedIn == 0)
					res.render('viewpost', {post: baz[i]});
				else
					res.render('viewpost', {post: baz[i], loggedIn: 'Yes'});
				 
			}
		}
	});
	
	}		
});

app.post('/comment', (req, res) =>
{
	if (loggedIn == 0)
	{
		res.render('login', {message: 'You must first login before you can comment on a post!'});	
	}
	else
	{
		res.render('newcomment', {id: req.body.postID, date: req.body.postDate, topic: req.body.postTopic, loggedIn: 'Yes'});
	}		
});

app.post('/newcomment', (req, res) =>
{	
	User.find({id: req.body.postID}).then((response) =>
	{	
		var baz = response[0].posts;
		
		for (var i = 0; i < baz.length; i++)
		{
			if (baz[i].topic == req.body.postTopic && baz[i].date == req.body.postDate)
			{
				var add = {id: req.session.user.id, body: req.body.body};
				
				baz[i].comments.unshift(add);
				
				User.findOneAndUpdate({id: req.body.postID}, {posts: baz}).then((response) =>
				{
					//stuff					
				});
				
				res.render('viewpost', {post: baz[i], loggedIn: 'Yes'});				
				 
			}
		}
	});		
});

app.post('/fooprofile', (req, res) =>
{
	User.find({id: req.body.postID}).then((response) =>
	{
		if (loggedIn == 0)
			res.render('viewprofile', {bar: response[0]});
		else
			res.render('viewprofile', {bar: response[0], loggedIn: 'Yes'});
	});
});

app.post('/search', (req, res) =>
{
	var people = [];
	var posts = [];

	User.find().then((response) =>
	{
		for (var i = 0; i < response.length; i++)
		{
			if (response[i].id == req.body.search)
			{
				people.push(response[i]);	
			}
			
			for (var j = 0; j < response[i].posts.length; j++)
			{
				if (response[i].posts[j].topic == req.body.search)
				{
					posts.push(response[i].posts[j]);
				}
			}
		}
		
		if (loggedIn == 0)
			res.render('viewsearch', {people: people, posts: posts});
		else
			res.render('viewsearch', {people: people, posts: posts, loggedIn: 'Yes'});
	});
});




app.listen(3000);
