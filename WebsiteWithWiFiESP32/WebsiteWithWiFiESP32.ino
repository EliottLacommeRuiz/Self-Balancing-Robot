#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "Access Point";     // Your WiFi SSID
const char* password = ""; // Your WiFi Password

WebServer server(80);  // Create a web server object that listens on port 80

float Kp = 0.0, Ki = 0.0, Kd = 0.0; // PID variables

// HTML code for the web page
const char* html = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 PID Controller</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
    function sendPIDValues() {
      var xhr = new XMLHttpRequest();
      var kp = document.getElementById("kpSlider").value;
      var ki = document.getElementById("kiSlider").value;
      var kd = document.getElementById("kdSlider").value;
      var url = `/setPID?kp=${kp}&ki=${ki}&kd=${kd}`;
      xhr.open("GET", url, true);
      xhr.send();
    }

    function updatePIDValues() {
      document.getElementById("kpValue").innerHTML = document.getElementById("kpSlider").value;
      document.getElementById("kiValue").innerHTML = document.getElementById("kiSlider").value;
      document.getElementById("kdValue").innerHTML = document.getElementById("kdSlider").value;
    }
  </script>
</head>
<body>
  <h1>ESP32 PID Controller</h1>
  <p>Kp: <span id="kpValue">0</span></p>
  <input type="range" min="0" max="100" step="0.1" value="0" id="kpSlider" oninput="updatePIDValues()">
  <p>Ki: <span id="kiValue">0</span></p>
  <input type="range" min="0" max="100" step="0.1" value="0" id="kiSlider" oninput="updatePIDValues()">
  <p>Kd: <span id="kdValue">0</span></p>
  <input type="range" min="0" max="100" step="0.1" value="0" id="kdSlider" oninput="updatePIDValues()">
  <br><br>
  <button onclick="sendPIDValues()">Send PID Values</button>
</body>
</html>)rawliteral";

void handleRoot() {
  server.send(200, "text/html", html);
}

void handleSetPID() {
  if (server.hasArg("kp")) Kp = server.arg("kp").toFloat();
  if (server.hasArg("ki")) Ki = server.arg("ki").toFloat();
  if (server.hasArg("kd")) Kd = server.arg("kd").toFloat();
  
  String message = "Received PID values - Kp: " + String(Kp) + ", Ki: " + String(Ki) + ", Kd: " + String(Kd);
  Serial.println(message);
  server.send(200, "text/plain", message);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  // This line prints the IP address

  server.on("/", handleRoot);
  server.on("/setPID", handleSetPID);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
