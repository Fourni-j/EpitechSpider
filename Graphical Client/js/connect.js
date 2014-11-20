
var socket = io.connect('http://127.0.0.1:42666');

var mine = document.getElementById("logs");

socket.on('listened', function (str) {
    console.log("finish");
    mine.innerHTML = str;
});
