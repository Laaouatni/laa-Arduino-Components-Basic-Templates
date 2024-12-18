/* written by Laaouatni Anas - https://github.com/Laaouatni/ */

#include <ESPAsyncWebServer.h>
#include <WiFi.h>
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  WiFi.begin("nomeWifi", "passwordWifi");
  while (WiFi.status() != WL_CONNECTED) {
  };
  Serial.println("WiFi connected! IP Address: " + WiFi.localIP().toString());

  // WiFi.mode(WIFI_AP);
  // WiFi.softAP("espWifi", "espPassword");
  // Serial.println("WiFi connected! IP hostpot Address: " + WiFi.softAPIP().toString());

  server.onNotFound([](AsyncWebServerRequest* request) {
    String urlPath = request->url();
    String valueToReturn;

    const int MAX_SPLITTED_PARTS = 3;
    int thisStartIndex = 0 + 1;
    int numberOfFoundedParts = 0;

    String foundedSplittedParts[MAX_SPLITTED_PARTS];

    for (int charIndex = thisStartIndex; charIndex < urlPath.length(); charIndex++) {
      const char thisChar = urlPath[charIndex];
      const bool isLastCharIteration = charIndex == urlPath.length() - 1;
      const bool isLastAvailablePartInArray = numberOfFoundedParts == MAX_SPLITTED_PARTS;
      const bool isThisCharSlash = thisChar == '/';
      if (isLastAvailablePartInArray || isLastCharIteration) {
        const int choosedEndIndex = isLastCharIteration && isThisCharSlash ? urlPath.length() - 1 : urlPath.length();
        foundedSplittedParts[numberOfFoundedParts] = urlPath.substring(thisStartIndex, choosedEndIndex);
        break;
      };
      if (!isThisCharSlash) continue;
      foundedSplittedParts[numberOfFoundedParts] = urlPath.substring(thisStartIndex, charIndex);
      numberOfFoundedParts++;
      thisStartIndex = charIndex + 1;
    };

    const String receivedCommandName = foundedSplittedParts[0];
    const int receivedPinId = foundedSplittedParts[1].toInt();
    const int receivedPinValue = foundedSplittedParts[2].toInt();

    if (receivedCommandName == "digitalRead") { // NO 5,RX0-3,TX0-1
      pinMode(receivedPinId, INPUT);
      request->send(200, "text/plain", String(digitalRead(receivedPinId)));
    };

    if (receivedCommandName == "analogRead") { // SI 33,32,35,34
      pinMode(receivedPinId, INPUT);
      request->send(200, "text/plain", String(analogRead(receivedPinId)));
    };

    if (receivedCommandName == "digitalWrite") { // NO 35,34
      pinMode(receivedPinId, OUTPUT);
      digitalWrite(receivedPinId, receivedPinValue);
      request->send(200, "text/plain", String(digitalRead(receivedPinId)));
    };

    if (receivedCommandName == "analogWrite") { // NO 27,26,25,33,35,34
      pinMode(receivedPinId, OUTPUT);
      analogWrite(receivedPinId, receivedPinValue);
      request->send(200, "text/plain", String(receivedPinValue));
    };

    request->send(404, "text/plain", "404");
  });

  server.begin();
}

void loop(){};
