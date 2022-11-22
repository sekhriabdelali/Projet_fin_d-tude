#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH THE MAC Address of your receiver
uint8_t noud1Address[] = {0xBC, 0xFF, 0x4D, 0x49, 0xEE, 0xE6};
uint8_t noud2Address[] = {0x30, 0x83, 0x98, 0xAA, 0xCF, 0x12};
uint8_t puisAddress[] = {0x84, 0xF3, 0xEB, 0xB7, 0x4D, 0x19};

// Define variables to store incoming readings
float incomingT, incomingT2;
float incomingH, incomingH2;
float incomingQ, incomingQ2;
String incomingID;
float tab_1[3], tab_2[3];
bool start_timeout = false;
unsigned long debut_of_timeout;
const unsigned long delay_of_timeout = 10000;
String retard = "";
int i;
String t, h, q;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message
{
    String id;
    float t;
    float h;
    float q;
} struct_message;

typedef struct send_message
{
    String id;
    float t1;
    float h1;
    float q1;
    float t2;
    float h2;
    float q2;
};

// Create a struct_message called DHTReadings to hold sensor readings
send_message message;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

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

// Callback when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
    Serial.print("Bytes received: ");
    Serial.println(len);
    incomingID = incomingReadings.id;
    incomingT = incomingReadings.t;
    incomingH = incomingReadings.h;
    incomingQ = incomingReadings.q;
    printIncomingReadings();
    Serial.print("\n retard: ");
    Serial.print(retard);
    Serial.print("\n");
    if (incomingID == "1")
    {
        if (retard != "1")
        {
            tab_1[0] = incomingT;
            tab_1[1] = incomingH;
            tab_1[2] = incomingQ;
            Serial.print("\nsave in tab1 1\n");
            if (!start_timeout)
            {
                debut_of_timeout = millis();
                start_timeout = true;
                Serial.print("\nstart timeout who:1 and debut is:");
                Serial.print(debut_of_timeout);
                Serial.println(" ");
            }
            else
            {
                send_to_puis();
            }
        }
        else
        {
            retard = "";
        }
    }

    if (incomingID == "2")
    {
        if (retard != "2")
        {
            tab_2[0] = incomingT;
            tab_2[1] = incomingH;
            tab_2[2] = incomingQ;
            Serial.print("\nsave in tab2 2\n");
            if (!start_timeout)
            {
                debut_of_timeout = millis();
                start_timeout = true;
                Serial.print("\nstart timeout who:2 and debut is:");
                Serial.print(debut_of_timeout);
                Serial.println(" ");
            }
            else
            {
                send_to_puis();
            }
        }
        else
        {
            retard = "";
        }
    }
}

void send_to_puis()
{

    if (tab_1[0] != 0.00 && tab_2[0] != 0.00)
    {
        message.id = "1j2";
        message.t1 = tab_1[0];
        message.h1 = tab_1[1];
        message.q1 = tab_1[2];
        message.t2 = tab_2[0];
        message.h2 = tab_2[1];
        message.q2 = tab_2[2];
        retard = "";
    }
    else
    {
        if (tab_1[0] != 0.00 && tab_2[0] == 0.00)
        {
            message.id = "1j2";
            message.t1 = tab_1[0];
            message.h1 = tab_1[1];
            message.q1 = tab_1[2];
            message.t2 = tab_2[0];
            message.h2 = tab_2[1];
            message.q2 = tab_2[2];
            retard = "2";
        }

        if (tab_1[0] == 0.00 && tab_2[0] != 0.00)
        {
            message.id = "1j2";
            message.t1 = tab_1[0];
            message.h1 = tab_1[1];
            message.q1 = tab_1[2];
            message.t2 = tab_2[0];
            message.h2 = tab_2[1];
            message.q2 = tab_2[2];
            retard = "1";
        }
    }
    // Send message via ESP-NOW
    esp_now_send(puisAddress, (uint8_t *)&message, sizeof(message));
    start_timeout = false;
    Serial.print("\nwe are send message to puis");
    empty_tabs();
    Serial.print("\ndata send to puis is:\n");
    Serial.println(message.id);
    Serial.println(message.t1);
    Serial.println(message.h1);
    Serial.println(message.q1);
    Serial.println(message.t2);
    Serial.println(message.h2);
    Serial.println(message.q2);
}

void empty_tabs()
{

    for (i = 0; i < 3; i++)
    {
        tab_1[i] = 0.00;
        tab_2[i] = 0.00;
    }
    Serial.print("\n tabs empty");
}

void printIncomingReadings()
{
    // Display Readings in Serial Monitor
    Serial.println("INCOMING READINGS");
    Serial.print("\nID: ");
    Serial.print(incomingID);
    Serial.print("\nTemperature: ");
    Serial.print(incomingT);
    Serial.println(" ºC");
    Serial.print("\nHumidity: ");
    Serial.print(incomingH);
    Serial.print("\nqualite_air: ");
    Serial.print(incomingQ);
    Serial.println(" %");
}

void setup()
{
    // Init Serial Monitor
    Serial.begin(9600);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Init ESP-NOW
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    empty_tabs();

    // Set ESP-NOW Role
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);

    // Register peer
    esp_now_add_peer(noud1Address, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    esp_now_add_peer(noud2Address, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    esp_now_add_peer(puisAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

    // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{

    if (start_timeout && (millis() - debut_of_timeout > delay_of_timeout))
    {

        send_to_puis();
    }
}