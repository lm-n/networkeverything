/*
    Demonstrates how to start a server, read inputs from attaches sensors
    and change the color of the page based on sensor data.
    On the NYU network, you'll need to do this locally.
*/

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x10, 0x3F, 0xE9
};
//IPAddress ip(192, 168, 1, 100);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("serial has started...");
  // start the Ethernet connection and the server:
  Ethernet.begin(mac);
  Serial.println("starting the server...");
  delay(1000);
  server.begin(); // start the server

  // print out some info to the serial monitor
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) { //  if someone connects
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read(); // read the request from the client
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {

          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          // The above lines let the browser know that it will receive a webpage

          // standard web formatting
          client.println("<html>");
          client.println("<head>");

          // HTML5 asks for CSS to change background color
          client.println("<style>");
          client.println("body{background-color: rgb(41, 41, 188); font-family: sans-serif; font-size: 30px; color: white}; text-align: center;");
          client.println("</style>");
          client.println("</head>");
          // close the <head> tag above
          // and start the body >below>
          client.println("<body>");

          if (digitalRead(8) == 0) {
            client.println("HELLO I AM UNDER THE WATER");
            client.println("Please help me I am drowning");
            client.println("Blblblblb");
            client.println("<iframe width='960' height='720' src='https://www.youtube.com/embed/z6-FWJteNLI?autoplay=1&rel=0&amp;controls=0&amp;showinfo=0' frameborder='0' allowfullscreen></iframe>");
            delay(10000);
          }
          else {
            client.println("Hello i am not under the water");
          }
          // close the boyd and end the page

          client.println("</body>");
          client.println("</html>");
          break; // escape!!!! This breaks us out of the while() started up above!
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
