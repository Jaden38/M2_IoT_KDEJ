#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define LED_PIN 2

const char* WIFI_SSID = "";
const char* WIFI_PASS = "";

DHT dht(DHTPIN, DHTTYPE);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
float temp, hum, heatIndex, tempF;

String getSensorDataJSON()
{
  String json = "{";
  json += "\"temperature_c\":" + String(temp) + ",";
  json += "\"temperature_f\":" + String(tempF) + ",";
  json += "\"humidity\":" + String(hum) + ",";
  json += "\"heat_index_c\":" + String(heatIndex);
  json += "}";
  return json;
}

void notifyClients()
{
  String message = getSensorDataJSON();
  ws.textAll(message);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  // Not used here, but you could parse messages from client
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
             AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    Serial.println("Client connected");
    client->text(getSensorDataJSON());
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.println("Client disconnected");
  }
  else if (type == WS_EVT_DATA)
  {
    handleWebSocketMessage(arg, data, len);
  }
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());
  ws.onEvent(onEvent);
  server.addHandler(&ws);

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", getSensorDataJSON());
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(
                  200, "text/html",
                  R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>ESP32 Climate Monitor</title>
        <style>
          * { margin: 0; padding: 0; box-sizing: border-box; }
          body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
          }
          .container {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 20px;
            padding: 40px;
            box-shadow: 0 20px 60px rgba(0, 0, 0, 0.3);
            max-width: 600px;
            width: 100%;
            animation: fadeIn 0.5s ease-in;
          }
          @keyframes fadeIn {
            from { opacity: 0; transform: translateY(20px); }
            to { opacity: 1; transform: translateY(0); }
          }
          h1 {
            text-align: center;
            color: #333;
            margin-bottom: 10px;
            font-size: 2em;
          }
          .subtitle {
            text-align: center;
            color: #666;
            margin-bottom: 30px;
            font-size: 0.9em;
          }
          .status {
            text-align: center;
            padding: 8px;
            border-radius: 8px;
            margin-bottom: 30px;
            font-weight: 600;
            background: #d4edda;
            color: #155724;
          }
          .status.disconnected {
            background: #f8d7da;
            color: #721c24;
          }
          .cards {
            display: grid;
            grid-template-columns: 1fr;
            gap: 20px;
          }
          .card {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            border-radius: 15px;
            padding: 25px;
            color: white;
            box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
            transition: transform 0.3s ease;
          }
          .card:hover {
            transform: translateY(-5px);
          }
          .card-icon {
            font-size: 2.5em;
            margin-bottom: 10px;
          }
          .card-label {
            font-size: 0.9em;
            opacity: 0.9;
            margin-bottom: 5px;
          }
          .card-value {
            font-size: 2.5em;
            font-weight: bold;
            margin-bottom: 5px;
          }
          .card-unit {
            font-size: 1.2em;
            opacity: 0.8;
          }
          .card.temp { background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%); }
          .card.humidity { background: linear-gradient(135deg, #4facfe 0%, #00f2fe 100%); }
          .card.heat { background: linear-gradient(135deg, #fa709a 0%, #fee140 100%); }
          @media (max-width: 600px) {
            .container { padding: 25px; }
            h1 { font-size: 1.5em; }
            .card-value { font-size: 2em; }
          }
        </style>
      </head>
      <body>
        <div class="container">
          <h1>🌡️ Climate Monitor</h1>
          <p class="subtitle">ESP32 DHT22 Real-time Sensor</p>
          <div id="status" class="status">Connecting...</div>
          <div class="cards">
            <div class="card temp">
              <div class="card-icon">🌡️</div>
              <div class="card-label">Temperature</div>
              <div class="card-value" id="temp">--</div>
              <div class="card-unit">°C</div>
            </div>
            <div class="card humidity">
              <div class="card-icon">💧</div>
              <div class="card-label">Humidity</div>
              <div class="card-value" id="hum">--</div>
              <div class="card-unit">%</div>
            </div>
            <div class="card heat">
              <div class="card-icon">🔥</div>
              <div class="card-label">Heat Index</div>
              <div class="card-value" id="heat">--</div>
              <div class="card-unit">°C</div>
            </div>
          </div>
        </div>
        <script>
          var gateway = `ws://${window.location.hostname}/ws`;
          var ws = new WebSocket(gateway);
          var statusEl = document.getElementById("status");
          
          ws.onopen = function() {
            statusEl.textContent = "✓ Connected";
            statusEl.classList.remove("disconnected");
          };
          
          ws.onclose = function() {
            statusEl.textContent = "✗ Disconnected";
            statusEl.classList.add("disconnected");
          };
          
          ws.onmessage = function(event) {
            let data = JSON.parse(event.data);
            document.getElementById("temp").textContent = data.temperature_c.toFixed(1);
            document.getElementById("hum").textContent = data.humidity.toFixed(1);
            document.getElementById("heat").textContent = data.heat_index_c.toFixed(1);
          };
        </script>
      </body>
      </html>
      )rawliteral"); });
  server.begin();
}

void loop()
{
  // Read sensor every 3s
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  tempF = dht.readTemperature(true);
  if (!isnan(hum) && !isnan(temp) && !isnan(tempF))
  {
    heatIndex = dht.computeHeatIndex(temp, hum, false);
    notifyClients(); // Push update to all connected browsers
  }
  delay(3000);
}