#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
// REPLACE WITH THE MAC Address of your receiver
uint8_t aggAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

const char *ssid = "pfe";
const char *password = "123456789";
// Your Domain name with URL path or IP address with path
const char *serverName = "http://192.168.5.3:80/update-sensor.php";
const String raspberrypi = "192.168.5.3";
const int httpPort = 80;
IPAddress Ip(192, 168, 5, 1);
IPAddress NMask(255, 255, 255, 0);
WiFiClient client;
HTTPClient http;
unsigned long debut_of_timeout;
const unsigned long delay_of_timeout = 10000;
bool arrive = false;

// Define variables to store incoming readings
String incomingID;
float incomingT1;
float incomingH1;
float incomingQ1;
float incomingT2;
float incomingH2;
float incomingQ2;
String id, t, h, q;

typedef struct struct_message
{
    String id;
    float t1;
    float h1;
    float q1;
    float t2;
    float h2;
    float q2;
} struct_message;

// Create a struct_message to hold incoming sensor readings
struct_message incomingmessage;

// Callback when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
    memcpy(&incomingmessage, incomingData, sizeof(incomingmessage));
    Serial.print("Bytes received: ");
    Serial.println(len);
    incomingID = incomingmessage.id;
    incomingT1 = incomingmessage.t1;
    incomingH1 = incomingmessage.h1;
    incomingQ1 = incomingmessage.q1;
    incomingT2 = incomingmessage.t2;
    incomingH2 = incomingmessage.h2;
    incomingQ2 = incomingmessage.q2;
    printIncomingReadings();
    t.concat(incomingT1);
    t = t + "j";
    t.concat(incomingT2);
    h.concat(incomingH1);
    h = h + "j";
    h.concat(incomingH2);
    q.concat(incomingQ1);
    q = q + "j";
    q.concat(incomingQ2);
    Serial.println(t);
    Serial.println(h);
    Serial.println(q);
    arrive = true;
    debut_of_timeout = millis();
}

void send_to_raspi()
{

    if (client.connect(raspberrypi, httpPort))
    {
        Serial.print("\nconnection succesful to raspberry pi");
        http.begin(client, serverName);

        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        // Data to send with HTTP POST
        String httpRequestData = "src=" + incomingID + "&temp=" + t + "&hum=" + h + "&qa=" + q;
        Serial.print("\n" + httpRequestData + "\n");
        // Send HTTP POST request
        int httpResponseCode = http.POST(httpRequestData);

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        // Free resources
        http.end();
        Serial.print("\npost request is: ");
        Serial.print(httpRequestData + "\n");
    }
    else
    {
        Serial.print("not connect to raspri");
    }

    t = "";
    h = "";
    q = "";
    arrive = false;
}

void printIncomingReadings()
{
    // Display Readings in Serial Monitor
    Serial.println("INCOMING READINGS");
    Serial.print("\nID: ");
    Serial.print(incomingID);
    Serial.print("\nTemperature: ");
    Serial.print(incomingT1);
    Serial.println(" ºC");
    Serial.print("\nHumidity: ");
    Serial.print(incomingH1);
    Serial.print("\nqualite_air: ");
    Serial.print(incomingQ1);
    Serial.println(" %");
    Serial.print("\nTemperature: ");
    Serial.print(incomingT2);
    Serial.println(" ºC");
    Serial.print("\nHumidity: ");
    Serial.print(incomingH2);
    Serial.print("\nqualite_air: ");
    Serial.print(incomingQ2);
    Serial.println(" %");
}

void setup()
{
    // Init Serial Monitor
    Serial.begin(9600);

    // Set device as a Wi-Fi Station + AP
    WiFi.mode(WIFI_AP_STA);

    // démarrer le point d'accès en utilisant le ssid et le password.

    Serial.print("\nconfiguring acess point....\n");
    WiFi.softAPConfig(Ip, Ip, NMask);
    while (WiFi.softAP(ssid, password) == false)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("configuring acess point successfully.");

    // Init ESP-NOW
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Set ESP-NOW Role
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

    // Once ESPNow is successfully Init, we will register for Send CB to

    // Register peer
    esp_now_add_peer(aggAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

    // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
    if (arrive && (millis() - debut_of_timeout > delay_of_timeout))
    {
        send_to_raspi();
    }
}