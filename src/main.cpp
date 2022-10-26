#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

const char *ssid = "Space Inc";
const char *password = "$m_w\"T!Lj.\'\\nzPB";

const char *read_url = "http://178.79.147.9:70/api/trpc/livestock.list?batch=1&input={%220%22:{%22json%22:{}}}";
const char *update_url = "http://178.79.147.9:70/api/trpc/livestock.list?batch=1&input={%220%22:{%22json%22:{}}}";

String httpGETRequest(const char *serverName);
void con_wifi();
void read_db(const char *url);


unsigned long prev_db_millis = 0UL;
unsigned long update_db_interval = 10000UL;

void setup()
{
  Serial.begin(9600);
  con_wifi();
}

void loop()
{
  unsigned long cur_millis = millis();

  if (cur_millis - prev_db_millis > update_db_interval)
  {
    read_db(read_url);
     prev_db_millis  = cur_millis;
  }
}

String httpGETRequest(const char *serverName)
{


float sensorReadingsArr[1000];

  WiFiClient client;
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(client, serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void con_wifi()
{
  delay(10);
  Serial.println('\n');

  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("-");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(++i);
    Serial.print("");
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
};

void read_db(const char *url)
{

  if (WiFi.status() == WL_CONNECTED)
  {
    String res = httpGETRequest(read_url);
    Serial.println(res);
    JSONVar myObject = JSON.parse(res);

    // if (JSON.typeof(myObject) == "undefined") {
    //   Serial.println("Parsing input failed!");
    //   return;
    // }

    Serial.print("JSON object = ");
    Serial.println(myObject);

    // // myObject.keys() can be used to get an array of all the keys in the object
    // JSONVar keys = myObject.keys();

    // for (int i = 0; i < keys.length(); i++) {
    //   JSONVar value = myObject[keys[i]];
    //   Serial.print(keys[i]);
    //   Serial.print(" = ");
    //   Serial.println(value);
    //   sensorReadingsArr[i] = double(value);
    // }
    // Serial.print("1 = ");
    // Serial.println(sensorReadingsArr[0]);
    // Serial.print("2 = ");
    // Serial.println(sensorReadingsArr[1]);
    // Serial.print("3 = ");
    // Serial.println(sensorReadingsArr[2]);
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
}
