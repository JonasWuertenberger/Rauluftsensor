#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_BME280.h>
#include <WiFi.h>
#include <WebServer.h>

// Display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Sensor
Adafruit_BME280 bme;

// WLAN-Daten
const char* ssid = "LAPTOP-VON-BOSS";
const char* password = "sagichdirnicht";

// Webserver
WebServer server(80);

// Wetterdaten
float temp, hum, pres;

String getHTML() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>🌐 Wetterstation</title>
  <style>
    @import url('https://fonts.googleapis.com/css2?family=Orbitron:wght@500&display=swap');

    body {
      margin: 0;
      padding: 30px 10px;
      font-family: 'Orbitron', sans-serif;
      background: linear-gradient(to top left, #0f0c29, #302b63, #24243e);
      color: #ffffff;
      text-align: center;
    }

    h2 {
      font-size: 2.5em;
      color: #00ffe5;
      text-shadow: 0 0 12px #00ffe5, 0 0 24px #00c8ff;
      margin-bottom: 40px;
    }

    .card-container {
      display: flex;
      justify-content: center;
      flex-wrap: wrap;
      gap: 20px;
    }

    .card {
      background: rgba(0, 0, 0, 0.3);
      border: 1px solid rgba(0, 255, 255, 0.2);
      border-radius: 20px;
      padding: 30px 40px;
      min-width: 200px;
      cursor: pointer;
      box-shadow: 0 0 15px rgba(0, 255, 255, 0.3);
      backdrop-filter: blur(12px);
      transition: transform 0.4s ease, box-shadow 0.4s ease;
    }

    .card:hover {
      transform: scale(1.08);
      box-shadow: 0 0 25px rgba(0, 255, 255, 0.5);
    }

    .value {
      font-size: 3em;
      color: #00e0ff;
      text-shadow: 0 0 10px #00ffff;
    }

    .unit {
      font-size: 1em;
      color: #aaa;
      margin-top: 8px;
      text-shadow: 0 0 5px #5eeaff;
    }

    .footer {
      margin-top: 50px;
      font-size: 0.9em;
      color: #999;
      text-shadow: 0 0 6px #777;
    }

    canvas {
      margin-top: 40px;
      background-color: #1a1a1a;
      border-radius: 12px;
      padding: 10px;
      box-shadow: 0 0 10px rgba(0,255,255,0.3);
    }

    .hidden {
      display: none;
    }
  </style>
</head>
<body>
  <h2>🌍 EPIC GLOBE-WETTERSTATION</h2>

  <div class="card-container">
    <div class="card" onclick="showChart('temp')">
      <div class="value" id="temp-value">--</div>
      <div class="unit">Temperatur</div>
    </div>
    <div class="card" onclick="showChart('hum')">
      <div class="value" id="hum-value">--</div>
      <div class="unit">Luftfeuchtigkeit</div>
    </div>
    <div class="card" onclick="showChart('pres')">
      <div class="value" id="pres-value">--</div>
      <div class="unit">Luftdruck</div>
    </div>
  </div>

  <canvas id="chartCanvas" class="hidden" width="600" height="300"></canvas>

  <div class="footer" id="footer">Live-Update alle 5 Sekunden</div>

  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <script>
    let tempHistory = [];
    let humHistory = [];
    let presHistory = [];

    function updateValues(data) {
      document.getElementById("temp-value").textContent = data.temp + " °C";
      document.getElementById("hum-value").textContent = data.hum + " %";
      document.getElementById("pres-value").textContent = data.pres + " hPa";

      tempHistory.push(data.temp);
      humHistory.push(data.hum);
      presHistory.push(data.pres);
    }

    function showChart(type) {
      const canvas = document.getElementById('chartCanvas');
      canvas.classList.remove('hidden');

      const ctx = canvas.getContext('2d');
      let history, label;

      if (type === 'temp') {
        history = tempHistory;
        label = 'Temperatur (°C)';
      } else if (type === 'hum') {
        history = humHistory;
        label = 'Luftfeuchtigkeit (%)';
      } else {
        history = presHistory;
        label = 'Luftdruck (hPa)';
      }

      if (window.myChart) {
        window.myChart.destroy();
      }

      window.myChart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: history.map((_, i) => i + 1),
          datasets: [{
            label: label,
            data: history,
            borderColor: '#00e0ff',
            backgroundColor: 'rgba(0,224,255,0.2)',
            fill: true,
            tension: 0.3
          }]
        },
        options: {
          responsive: true,
          plugins: {
            legend: { labels: { color: "#ffffff" } }
          },
          scales: {
            x: { ticks: { color: "#ffffff" } },
            y: { ticks: { color: "#ffffff" } }
          }
        }
      });
    }

    function fetchData() {
      fetch("/data")
        .then(res => res.json())
        .then(data => {
          updateValues(data);
          setTimeout(fetchData, 5000);
        });
    }

    fetchData();
  </script>
</body>
</html>
  )rawliteral";
  return html;
}

void setup() {
  Wire.begin();
  u8g2.begin();
  Serial.begin(115200);

  if (!bme.begin(0x76)) {
    Serial.println("BME280 nicht gefunden!");
    while (1);
  }

  WiFi.begin(ssid, password);
  Serial.print("Verbinde mit WLAN");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nVerbunden! IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/html", getHTML());
  });

  server.on("/data", []() {
    temp = bme.readTemperature();
    hum = bme.readHumidity();
    pres = bme.readPressure() / 100.0F;

    String json = "{";
    json += "\"temp\":" + String(temp, 1) + ",";
    json += "\"hum\":" + String(hum, 1) + ",";
    json += "\"pres\":" + String(pres, 1);
    json += "}";
    server.send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  server.handleClient();

  // Sensorwerte lesen
  float t = bme.readTemperature();
  float h = bme.readHumidity();
  float p = bme.readPressure() / 100.0;

  // Begrenzen für Darstellung
  t = constrain(t, 0, 50);
  h = constrain(h, 0, 100);
  p = constrain(p, 900, 1100);

  // Balkenbreiten berechnen (max. 60 px)
  int t_bar = map(t, 0, 50, 0, 60);
  int h_bar = map(h, 0, 100, 0, 60);
  int p_bar = map(p, 900, 1100, 0, 60);

  u8g2.clearBuffer();

  // Titel mittig oben
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.drawStr(34, 10, "WETTERSTATION");

  // Temperatur – Zeile 1
  u8g2.setFont(u8g2_font_7x13B_tr);
  u8g2.drawStr(4, 26, "T:");
  u8g2.setCursor(20, 26);
  u8g2.print(t, 1);
  u8g2.print(" C");
  u8g2.drawFrame(70, 18, 60, 8);
  u8g2.drawBox(70, 18, t_bar, 8);

  // Luftfeuchtigkeit – Zeile 2
  u8g2.drawStr(4, 42, "H:");
  u8g2.setCursor(20, 42);
  u8g2.print(h, 1);
  u8g2.print(" %");
  u8g2.drawFrame(70, 34, 60, 8);
  u8g2.drawBox(70, 34, h_bar, 8);

  // Luftdruck – Zeile 3
  u8g2.drawStr(4, 58, "P:");
  u8g2.setCursor(20, 58);
  u8g2.print(p, 0);
  u8g2.print(" h");
  u8g2.drawFrame(70, 50, 60, 8);
  u8g2.drawBox(70, 50, p_bar, 8);

  u8g2.sendBuffer();
  delay(2000);
}
