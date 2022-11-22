#include <ESP8266WiFi.h>
#include <espnow.h>

// library dht 11
#include <DHT.h>

const byte DHT_PIN = D1;  // la broche numérique à laquelle nous sommes connectés.
const byte DHT_type = 11; // nous utilisons Capteur DH11.
float h = 0.0, t = 0.0, q = 0.0;
String id = "1";
DHT dht(DHT_PIN, DHT_type);

// REPLACE WITH THE MAC Address of your aggregator
uint8_t aggAddress[] = {0x84, 0xF3, 0xEB, 0xE3, 0x40, 0xF1};

// Updates DHT readings every 10 seconds
const long interval = 30000;
unsigned long previousMillis = 0; // will store last time DHT was updated

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message
{
    String id;
    float t;
    float h;
    float q;
} struct_message;

// Create a struct_message called Readings to hold sensor readings
struct_message SensorReadings;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
    Serial.print("Last Packet Send Status: ");
    if (sendStatus == 0)
    {
        Serial.println("Delivery success");
    }
    else
    {
        Serial.println("Delivery fail");
    }
}

void getReadings()
{
    // Read Temperature
    t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    // float t = dht.readTemperature(true);
    if (isnan(t))
    {
        Serial.println("Failed to read from DHT");
        t = 0.0;
    }
    h = dht.readHumidity();
    if (isnan(h))
    {
        Serial.println("Failed to read from DHT");
        h = 0.0;
    }
    q = analogRead(A0);
    if (isnan(q))
    {
        Serial.println("Failed to read from DHT");
        q = 0.0;
    }
}

void setup()
{
    // Init Serial Monitor
    Serial.begin(9600);

    // Init DHT sensor
    dht.begin();

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Init ESP-NOW
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Set ESP-NOW Role
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);

    // Register peer
    esp_now_add_peer(aggAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}

void loop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        // save the last time you updated the DHT values
        previousMillis = currentMillis;

        // Get DHT readings
        getReadings();

        // Set values to send
        SensorReadings.id = id;
        SensorReadings.t = t;
        SensorReadings.h = h;
        SensorReadings.q = q;

        // Send message via ESP-NOW
        esp_now_send(aggAddress, (uint8_t *)&SensorReadings, sizeof(SensorReadings));
    }
}