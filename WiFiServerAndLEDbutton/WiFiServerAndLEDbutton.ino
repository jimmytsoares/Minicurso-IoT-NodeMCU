#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

const char* ssid = "Curso-PET-ESP";
const char* password = "12345678";
ESP8266WebServer server(80);    // Cria o web server para http requests na porta 80

void handleRoot();    
void handleLEDon();
void handleLEDoff();
void handleNotFound();

void setup(void){
  Serial.begin(115200);         // para mandar msgs pro pc
  delay(10);
  Serial.println('\n');

  pinMode(LED_BUILTIN, OUTPUT); //define o led da placa como saida

  WiFi.mode(WIFI_STA); //seta como modo station
  WiFi.begin(ssid, password); //conecta na rede
  Serial.println("Connecting ...");
  
  while (WiFi.status() != WL_CONNECTED) { //espera a conexao
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // mostra a rede conectada
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // o endereço ip do esp nessa rede

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);     // chamado no root, pagina inicial
  server.on("/LEDon", HTTP_POST, handleLEDon);  // chama a funcao handleLEDon quando a pagina estiver em /LEDon
  server.on("/LEDoff", HTTP_POST, handleLEDoff); //mesmo de cima
  server.onNotFound(handleNotFound);        // para qualquer outra pagina que nao ta especificado, erro 404

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients
}

void handleRoot() {                         // When URI / is requested, send a web page with a button to toggle the LED
  String html ="<!DOCTYPE html> <html> <head> <style> .button { background-color: #4CAF50; border: none; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; border-radius: 12px; } </style> </head> <body> <h1 style=\"text-align: center;\"><font face=\"helvetica\">Minicurso IoT NodeMCU</font></h1> <h2 style=\"text-align: center;\"><strong><font face=\"helvetica\" color=\"green\">LED da placa</font></strong></h2> <form action=\"/LEDon\" method=\"post\"> <center> <button class=\"button\">ON</button></br> <button class=\"button\" formaction=\"/LEDoff\">OFF</button> </center> </form> </body> </html>";
  server.send(200, "text/html", html);
}

void handleLEDon() {                          // If a POST request is made to URI /LED
  digitalWrite(LED_BUILTIN,LOW);      // Change the state of the LED
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void handleLEDoff() {                          // If a POST request is made to URI /LED
  digitalWrite(LED_BUILTIN,HIGH);      // Change the state of the LED
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
