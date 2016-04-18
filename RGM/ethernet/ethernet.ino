/*
  Based on WebClient example
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe, based on work by Adrian McEwen
  modified April 2016 by James Hosken
*/

#include <SPI.h>
#include <Servo.h>
#include <Ethernet.h>

// CHANGE THIS TO YOUR MAC ADDRESS                                        <<<<<<<<
byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0x40, 0xC4 };

//This is where we're connecting to.
char server[] = "im-rubegoldberg.herokuapp.com";

//Pretty useless I think. But sketch cries without it. Maybe it's your IP?
IPAddress ip(10, 0, 0, 127);

// Initialize the Ethernet client library
EthernetClient client;

String lastIn = "";         //Useful later, for handling server response.
bool isConnecting = false;  //Used to stop requests while reading from server.
int counter = 0;            //How long have I been going?


String deviceID = "AB1";           //Change this to your device ID (Based on when in the sequence you are)
//AB0 = Abu Dhabi Begin 0, SE1 = Shanghai End 1, etc.
bool RUNNING = false;         //Change this depending on whether you're Begin or End point
//Begin points set to false,
//End points set to true.
Servo startservo;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}  //Silly thing to Leonardo-proof it
  setupEthernet();
  startservo.attach(9);
  startservo.write(0);
}

void loop() {

  if (RUNNING) {
    //Do your app here.
    Serial.println("Running!");
    startservo.write(180);
    //IFF APP IS END POINT:
    //When Done:
    //RUNNING = false;
  } else {
    //Connect to server and such
    determineRequest();
  }
  counter++;
}

void determineRequest() {
  //Arbitrary reconnection threshold = 120
  if (counter % 120 == 0) {
    request();
    isConnecting = true;
  } else {
    connection();
  }
}


///// DO NOT MODIFY THE FOLLOWING
//////////////////////////////////

void setupEthernet() {
  Serial.println("Establishing Ethernet Connection");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(300);
  Serial.println("connecting...");
}
void request() {
  //Please, sir(ver), may I have some more?
  if (client.connect(server, 80)) {
    Serial.println("Request connected");

    // Make a HTTP request, with our unipque ID in the url:
    client.println(String("GET /devices/") + deviceID + String(" HTTP/1.1"));
    client.println("Host: im-rubegoldberg.herokuapp.com");
    client.println("Connection: close");
    client.println();

    Serial.println("Request sent!");
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}
void disconnection() {
  //Handle disconnection.
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    Serial.println("disconnected!");
    isConnecting = false;
    //RESTART
    Serial.println("Restarting!");
    setupEthernet();
  }
}
void connection() {
  // if there are incoming bytes available
  // from the server, read them and determine server's response.
  connectionRead();
  // if the server's disconnected, stop the client:
  if (isConnecting) {
    disconnection();
  }
}
//THIS IS WHERE THE MAGIC HAPPENS!
void connectionRead() {
  //Figure out what the server has said to us!
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
    //Last thing that we read will be the single char that the server responds with. Nevermind all the connection info nonsense.
    lastIn = c;
  } else {
    Serial.println("Last in" + lastIn);

    if (lastIn == "1" && RUNNING == false) {
      //For "Begin Points", we not set the run bool to true, so app can run its course.
      RUNNING = true;
    }
  }
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
