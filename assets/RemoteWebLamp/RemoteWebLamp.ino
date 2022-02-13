#include <SPI.h>
#include <Ethernet.h>

byte mac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 125);
EthernetServer server(80);

int Led = 7;
String statusLed = "OFF";
 
void setup() {
  pinMode(Led,OUTPUT);
  delay(10);
  
  Serial.begin(9600);

  Ethernet.begin(mac, ip);
  server.begin();
  
  Serial.print("Server is at:  ");
  Serial.println(Ethernet.localIP());  
}
 
void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println(F("\nNuevo cliente\n"));
    boolean Blanks = true;
    String cadena;
    
    while (client.connected()) {
      if (client.available()) {
        char r = client.read(); // Request
        Serial.write(r);
        cadena.concat(r);

        int Position1 = cadena.indexOf("Led=");
 
        if(cadena.substring(Position1) == "Led=ON") {
          digitalWrite(Led, HIGH);
          statusLed="ON";
        }
        if(cadena.substring(Position1)=="Led=OFF") {
          digitalWrite(Led,LOW);
          statusLed="OFF";
        }

        if (r == '\n' && Blanks) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();          

          client.println("<!DOCTYPE html>");
          client.println("<html lang='en'>");
          client.println("<head>");
            client.println("<meta charset='UTF-8'>");
            client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
            client.println("<link rel='shortchut icon' type='image/x-icon' href='https://brianlewyn.github.io/RemoteWebLamp/assets/img/BrianLewyn.ico'>");
            client.println("<link rel='stylesheet' href='https://brianlewyn.github.io/RemoteWebLamp/assets/style.css'>");
            client.println("<title>Remote Web Lamp</title>");
          client.println("</head>");
          client.println("<body>");

            client.println("<a href='192.168.0.125'>");
              client.println("<img src='https://brianlewyn.github.io/RemoteWebLamp/assets/img/BrianLewyn.svg'>");
            client.println("</a>");
            
            client.println("<div class='box'>");
              client.println("<h1>Remote Web Lamp</h1>");
              
              client.println("<div class='controls'>");
                client.println("<h2>Btn Led Lamp</h2>");
                
                client.println("<div class='btns'>");  
                  client.println("<button class='btn:ON' onClick=location.href='./?Led=ON\'>ON</button>");
                  client.println("<button class='btn:OFF' onClick=location.href='./?Led=OFF\'>OFF</button>");
                client.println("</div>");
                
                client.print("<h3 class='status:");
                  client.print(statusLed);
                    client.print("'>Status: ");
                      client.print(statusLed);
                client.println("</h3>");
              client.println("</div>");
            client.println("</div>");
            
            client.println("<div class='frame'>");
              client.print("<div class='img:");
                client.print(statusLed);
              client.println("'></div>");
            client.println("</div>");

          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (r == '\n') {Blanks = true;}
        else if (r != '\r') {Blanks = false;}
      }
    }
    delay(10);
    client.stop();
  }
}