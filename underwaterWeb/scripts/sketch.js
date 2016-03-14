// Got help from Craig Protzel: https://github.com/craigprotzel/Mashups/tree/master/07_Animation_Playstation/p5_spaceData_objects
// Thanks Craig!

var canvasElement;
var wataHeight = 300;
var count = 0;
var time;
var backgroundColor;
var oldNumber = 0; var oldTime = "00:00:00"; var newNumber = 0; var newTime = "00:00:00";
var strokeWidth = 10;
var speech0 = "Hello. Pls drink water. I'm drowning.";
var speech1 = "Drink 1/2 cup of H2O every 4 hours.";
var speech2 = "Your cup empty. Fill it up!";
var speech3 = "Place cup on sensor when not drinking.";
var milliseconds = 0;

/*
1. Nothing - 1023
2. Only circle - 842/794/804/814/824
3. Only cup - 647/617
4. Half cup - 455
5. Full cup (600ml) - 390/401
6. Min value - 218
7. A little bit more than half - 451
8. A little bit - 539
*/

// Define a global object to store the water data
var wataData = {
	ready : false,
	animate : false,
	apiData	: {},
	number : 0,
	at : 0
};

//Define a function to get the data
function getWataData() {
	// Using the xively library
	// Got help from here: http://stackoverflow.com/questions/18958581/xively-get-datastream-list-from-a-feed-using-javascript

	xively.setKey("4AHrvpHgYXWEProtqbL3tqiuiyFFTt2WUJQqj4XkomdRSB5w");
	xively.datastream.list(
    138535803               //id from the tutorial example
	, function (data) {
    data.forEach(function (datastream) {
    	console.log("doing this");
    	wataData.apiData = data;
    	wataData.number = data[0]["current_value"];
    	wataData.at = data[0]["at"].substring(11,19);
    	wataData.ready = true;
    });
});
}

/*---------------------------------------------
p5 Code
----------------------------------------------*/

function preload() {
	mySound = loadSound('media/underwata.mp3');
}

function setup() {
	console.log("Setup");
	// msg = createDiv('Getting the Wata level');
	var canvas = createCanvas(windowWidth, windowHeight);
	canvasElement = canvas.elt;
	manImg = loadImage("media/waterperson.png");  // Load the image
	// Using the p5 gif library: https://github.com/antiboredom/p5.gif.js
	wataGif = loadGif("media/water.gif");  // Load the gif
	speechBubble = loadImage("media/speechbubble.png");
	moon = loadImage("media/moon.gif");
	sunImg = loadImage("media/sun.gif");
	// The height of this image is 600
	console.log(canvasElement);
	backgroundColor = color(20, 40, 90);
}

function draw() {
	var thisTime = computeTime();
	var img = drawSunMoon(thisTime);
	background(backgroundColor);
	if (milliseconds%10000 < 25){
		getData();
		doNumberCalculations();
	}
	wataHeight = waterHeight();
	drawCup();
	milliseconds = millis();
	image(img, 10, 10);
	image(wataGif, windowWidth/2-wataGif.width/2, windowHeight - wataHeight - strokeWidth);
	image(manImg, windowWidth/2-manImg.width/2, windowHeight-manImg.height - strokeWidth);
	line(windowWidth/2-wataGif.width/2 - strokeWidth/2, windowHeight - strokeWidth/2, windowWidth/2+wataGif.width/2 + strokeWidth/2, windowHeight - strokeWidth/2);
	drawSpeechBubble();
}

//Execute the function to get the data at time intervals
function getData(){
	console.log("Getting data!");
	getWataData();
	console.log("Got number: " + wataData.number);
	console.log("Got time: " + wataData.at);
	if (wataData.number < 500 && wataData.number > 300){
		mySound.play();
	}
}

function doNumberCalculations(){
}

function computeTime(){
	var currentTime, hour, minute, second;
	hour = new Date().getHours(); minute = new Date().getMinutes(); second = new Date().getSeconds();

	hour = (hour < 10)? "0"+hour: hour;
	minute = (minute < 10)? "0"+minute: minute;
	second = (second < 10)? "0"+second: second;

	currentTime = hour + ":" + minute + ":" + second;
	return currentTime;
}

function drawCup(){
	fill(backgroundColor);
	strokeWeight(strokeWidth);
	rect(windowWidth/2-wataGif.width/2 - strokeWidth/2,  windowHeight - wataGif.height -  strokeWidth, wataGif.width + strokeWidth, wataGif.height + strokeWidth);
	ellipse(windowWidth/2, windowHeight - wataGif.height - strokeWidth, wataGif.width + strokeWidth, 50);
}

function drawSpeechBubble(){
	image(speechBubble, windowWidth*0.7, windowHeight/7);
	fill(0);
	textSize(32);
	text(updateText(), windowWidth * 0.75, windowHeight/4, 200, 150);
	fill(255);
	strokeWeight(strokeWidth/2);
	ellipse(windowWidth/2+100, windowHeight/2, 20,20);
	ellipse(windowWidth/2+150, windowHeight/2 - 25, 40,40);
	ellipse(windowWidth/2+225, windowHeight/2 - 60, 60,60);
}

function drawSunMoon(time){
	if (time.substring(0,2) <= 6 || time.substring(0,2) >= 20){
		time = "night";
		backgroundColor = color(20, 40, 90);
		return moon;
	}
	else {
		time = "day";
		backgroundColor = color(250, 250, 50);
		return sunImg;
	}
}

function updateText(){
	var level = wataData.number;

	if (level > 755){
		return speech3;
	}
	else if (level > 600 && level < 755){
		return speech2;
	}
	else if (level > 500 && level < 600){
		return speech1;
	}
	else {
		return speech0;
	}
}

function waterHeight(){
	/*Map to heights
	650 - 600
	350 - 0
	*/
	// return wataGif.height/2;
	return ((1 - ((wataData.number - 375)/300)) * 600);
}

function mouseClicked(){
	// console.log(mouseX, mouseY);
}

function keyPressed(){
	// console.log("The Key Code: " + keyCode);
}

function windowResized() {
	resizeCanvas(windowWidth, windowHeight);
	// console.log(width + " : " + height);
}

function timeDifference(oldTime, newTime){

	var oldH = oldTime.substring(0,2); var oldM = oldTime.substring(3,5); var oldS = oldTime.substring(6,8);
	var newH = newTime.substring(0,2); var newM = newTime.substring(3,5); var newS = newTime.substring(6,8);
	var retS = 0, retM = 0, retH = 0, retTime;

	retS += (oldS > newS)? 60 - (oldS - newS): newS - oldS;
	newM -= (oldS > newS)? 1: 0;

	retM += (oldM > newM)? 60 - (oldM - newM): newM - oldM;
	newH -= (oldM > newM)? 1: 0;

	retH += (oldH > newH)? 24 - (oldH - newH): newH - oldH;

	retH = (retH < 10)? "0"+retH: retH;
	retM = (retM < 10)? "0"+retM: retM;
	retS = (retS < 10)? "0"+retS: retS;

	retTime = retH + ":" + retM + ":" + retS;
	return retTime;
}