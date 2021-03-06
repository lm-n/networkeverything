import processing.serial.*;
import ddf.minim.*;

Minim minim;
AudioPlayer sound1;
AudioPlayer sound2;
AudioPlayer sound3;
AudioPlayer sound;
AudioPlayer[] sounds = new AudioPlayer[4];

Serial myPort;

String resultString;

PImage img1;
PImage img2;
PImage img3;

float leftPaddle, rightPaddle;
int resetButton, serveButton;
int leftPaddleX, rightPaddleX;
int paddleHeight = 50;
int paddleWidth = 10;
int imgNum = 3;

float leftMinimum = 120;
float rightMinimum = 100;
float leftMaximum = 530;
float rightMaximum =500;

int ballSize = 40;
int xDirection = 1;
int yDirection = 1;
int xPos, yPos;

boolean ballInMotion = false;
int leftScore = 0;
int rightScore = 0;

int fontSize = 36;

void setup(){
  size(640, 480);
  
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);
  
  myPort.bufferUntil('\n');
  
  leftPaddle = height/2;
  rightPaddle = height/2;
  resetButton = 0;
  serveButton = 0;
  
  leftPaddleX = 50;
  rightPaddleX = width - 50;
  
  noStroke();
  
  xPos = width/2;
  yPos = height/2;
  
  PFont myFont = createFont(PFont.list()[2], fontSize);
  textFont(myFont);
  img3 = loadImage("data/Luigi.png");
  img1 = loadImage("data/abhi.png");
  img2 = loadImage("data/scotto.png");
  
  minim = new Minim(this);
  //animal sounds: http://www.orangefreesounds.com/
  sound1 = minim.loadFile("data/chicken.mp3");
  sound2 = minim.loadFile("data/hyena.mp3");
  sound3 = minim.loadFile("data/sheep.mp3");
  sound = minim.loadFile("data/ParovStelar.mp3"); //From here: http://mp3pm.name/
  sound.loop();
  sounds[0] = sound;
  sounds[1] = sound1;
  sounds[2] = sound2;
  sounds[3] = sound3;
}

void draw() {
  background(#044f6f);
  fill(#ffffff);
  rect(leftPaddleX, leftPaddle, paddleWidth, paddleHeight);
  rect(rightPaddleX, rightPaddle, paddleWidth, paddleHeight);
  
  if (ballInMotion == true){
    animateBall();
  }
  if (serveButton == 1){
    ballInMotion = true;
  }
  if (resetButton ==1){
    leftScore = 0;
    rightScore = 0;
    ballInMotion = true;
  }
  
  if (resultString != null) {
    //text(resultString, 10, height/2);
  }
  
  text(leftScore, fontSize, fontSize);
  text(rightScore, width-fontSize, fontSize);
}

void serialEvent(Serial myPort){
  String inputString = myPort.readStringUntil('\n');
  //println(inputString);
  
  inputString = trim(inputString);
  resultString = "";
  
  int sensors[] = int(split(inputString, ','));
  println(int(split(inputString, ',')));
  
  if (sensors.length == 5){
    leftPaddle = map(sensors[1], leftMinimum, leftMaximum, 0, height);
    rightPaddle = map(sensors[0], rightMinimum, rightMaximum, 0, height);
    resetButton = sensors[2];
    serveButton = sensors[3];
    imgNum = sensors[4];
    for (int i = 1; i <4; i ++){
      sounds[i].pause();
    }
    sounds[imgNum].play();
    resultString += "left: " + leftPaddle + "\tright: " + rightPaddle;
    resultString += "\treset: " + resetButton + "\tserve:" + serveButton;
  }
  myPort.write('\r');
}

void animateBall() {
  if (xDirection < 0){
    if ((xPos <= leftPaddleX)){
      if((leftPaddle - (paddleHeight/2) <= yPos) && (yPos <= leftPaddle + (paddleHeight/2))){
        xDirection = -xDirection;
      }
    }
  }
  else{
    if ((xPos >= (rightPaddleX + ballSize/2))){
      if((rightPaddle - (paddleHeight/2) <= yPos) && (yPos <= rightPaddle + (paddleHeight/2))){
        xDirection = -xDirection;
      }  
    }
  }
  if (xPos < 0) {
    rightScore++;
    resetBall();
  }
  if (xPos > width){
    leftScore++;
    resetBall();
  }
  if ((yPos - ballSize/2 <=0) || (yPos + ballSize/2 >=height)){
    yDirection = -yDirection;
  }
  xPos = xPos + xDirection;
  yPos = yPos + yDirection;
  if (imgNum == 1){
    image(img1, xPos, yPos, ballSize, ballSize);
  }
  else if (imgNum == 2){
    image(img2, xPos, yPos, ballSize, ballSize);
  }
  else {
    image(img3, xPos, yPos, ballSize, ballSize);
  }

}

void resetBall(){
  xPos = width/2;
  yPos = height/2;
}