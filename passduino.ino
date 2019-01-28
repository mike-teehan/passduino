/*
  Passduino
  This sketch use the WiFi server capability of a MKR1000 to post a web interface
  that can trigger keyboard macros
  
  Required hardware:
  - Arduino/Genuino MKR1000;
  created 27 Jan 2019
  by Mike Teehan <mike.teehan@gmail.com>
*/

#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <Keyboard.h>
#include <ArduinoMDNS.h>
// https://github.com/llelundberg/EasyWebServer hacked for wifi
#include "EasyWebServer.h"

#include "passduino_resources.h"

// https://github.com/cmaglie/FlashStorage
// #include <FlashStorage.h>
#define MACRO_NAME_LENGTH 32
#define MACRO_LENGTH 100
typedef struct {
  char name[MACRO_NAME_LENGTH];
  char action[MACRO_LENGTH];
} Macro;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
#include "passduino_secrets.h"
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

char mdnsName[] = "passduino"; // the MDNS name that the board will respond to
// Note that the actual MDNS name will have '.local' after it

int status = WL_IDLE_STATUS;

// Create a MDNS responder to listen and respond to MDNS name requests.
WiFiUDP udp;
MDNS mdns(udp);

WiFiServer server(80);

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // initialize control over the keyboard:
  Keyboard.begin();

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the status:
  printWiFiStatus();

  server.begin();

  // Setup the MDNS responder to listen to the configured name.
  // NOTE: You _must_ call this _after_ connecting to the WiFi network and
  // being assigned an IP address.
  if (!mdns.begin(WiFi.localIP(), mdnsName)) {
    Serial.println("Failed to start MDNS responder!");
    while (1);
  }

  Serial.print("Server listening at http://");
  Serial.print(mdnsName);
  Serial.println(".local/");
  Serial.println();

  mdns.addServiceRecord("passduino._http", 80, MDNSServiceTCP);
}

void loop() {
  // Call the update() function on the MDNS responder every loop iteration to
  // make sure it can detect and respond to name requests.
  mdns.run();

  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    EasyWebServer w(client);
    w.serveUrl("/", rootPage);
    w.serveUrl("/macro", triggerMacro);
  }
}

void rootPage(EasyWebServer &w) {
  Serial.println("rootPage()");
  String page_html(PAGE_HTML);
  String macro_row(PAGE_HTML_MACRO_ROW);
  String tmp_macro_row = "";
  String macro_html = "";
  Macro m;
  for(int i = 0; i < MACRO_COUNT; i++) {
    m = macros[i];
    tmp_macro_row = macro_row;
    tmp_macro_row.replace("%%name%%", m.name);
    macro_html += tmp_macro_row;
  }
  page_html.replace("%%macros%%", macro_html);
  int pages = ceil(page_html.length() / 40);
  for(int i = 0; i < pages; i++) {
    if(i == pages - 1)
      w.client.println(page_html.substring(i * 40));
    else
      w.client.print(page_html.substring(i * 40, (i * 40) + 40));
  }
  //w.client.println(page_html);
  Serial.println("page_html:");
  Serial.println(page_html);
}

void triggerMacro(EasyWebServer &w) {
  Serial.println("triggerMacro()");
  Macro m;
  for(int i = 0; i < MACRO_COUNT; i++) {
    m = macros[i];
    if(strcmp(w.querystring, m.name) == 0) {
      Serial.println("action:");
      Serial.println(m.action);
      Serial.println();
      Keyboard.print(m.action);
    }
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
