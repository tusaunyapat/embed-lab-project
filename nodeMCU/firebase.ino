#include <ESP8266WiFi.h>
// #include <FirebaseESP8266.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "time.h"
#include <SoftwareSerial.h>
#include <string.h>

#define SSID "bonussy"
#define PASSWORD "bonus2547"
#define FIREBASE_API_KEY "AIzaSyAd_j28dAgEdm-U6lCyy2MjhviRR1KaxwE"
#define FIREBASE_EMAIL "test@test.com"
#define FIREBASE_PASSWORD "12345678"
#define FIREBASE_DATABASE_URL "https://embed-cc515-default-rtdb.asia-southeast1.firebasedatabase.app/"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Database path
String tempPath = "/temp";
String humidityPath = "/humidity";
String dustPath = "/dust";

// Counter
FirebaseJson json;

unsigned long sendDataPrevMillis = 0;
unsigned long getDataPrevMillis = 0;
unsigned long timerDelay = 15000;
unsigned long getDelay = 5000;

float dust = 0;
float humidity = 0;
float temp = 0;
String humidityData,tempData;
String dustData;

#define USE_AVG
const int sharpLEDPin = D4; 
const int sharpVoPin = D5; 
#ifdef USE_AVG

#define N 100
static unsigned long VoRawTotal = 0;
static int VoRawCount = 0;
#endif 
 
static float Voc = 0.6;
const float K = 0.5; 

const char* ntpServer = "pool.ntp.org";

// SoftwareSerial comm(D7, D8);
SoftwareSerial comm(D1, D2); // RX, TX

void initialUSBSerial() {
  // Start USB serial
}

void initialSTMSerial() {
  // Start communicate with STM32
}
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return (0);
  }
  time(&now);
  return now;
}

void initialWifi() {
  // Connect wifi with ssid and password
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting Wifi...  ");
    // Serial.printf("Connection Status: %d\n", WiFi.status());
    delay(1000);
  }

  // Connected
  Serial.print("Wi-Fi connected.");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
}

void initialTime() {
  // Start connect timestamp server
  configTime(0, 0, ntpServer);
}

void initialFirebase() {
  // Firebase client version
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  // Connect Firebase
  config.api_key = FIREBASE_API_KEY;
  config.database_url = FIREBASE_DATABASE_URL;

  // Assign the user sign in credentials
  auth.user.email = FIREBASE_EMAIL;
  auth.user.password = FIREBASE_PASSWORD;

  // Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback;

  // Begin with anonymous user
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void setup() {
  Serial.begin(115200);

  comm.begin(115200);
  // pinMode(D7,INPUT);
  // pinMode(D8,OUTPUT);

  // pinMode(D0,OUTPUT);

  pinMode(sharpLEDPin, OUTPUT);

  initialWifi();
  initialTime();
  initialFirebase();
}

// dust
float DustZ(){
  digitalWrite(sharpLEDPin, LOW);
  delayMicroseconds(280);
 
  int VoRaw = analogRead(sharpVoPin);
 
  digitalWrite(sharpLEDPin, HIGH);
  delayMicroseconds(9620);
 
  #ifdef PRINT_RAW_DATA
  Serial.println("");
  #endif // PRINT_RAW_DATA
 
  float Vo = VoRaw;
  #ifdef USE_AVG
  VoRawTotal += VoRaw;
  VoRawCount++;
  if ( VoRawCount >= N ) {
  Vo = 1.0 * VoRawTotal / N;
  VoRawCount = 0;
  VoRawTotal = 0;
  } else {
    return -1;
  }
  #endif 
 
  Vo = Vo / 1024.0 * 5.0;
 
  float dV = Vo - Voc;
  if ( dV < 0 ) {
    dV = 0;
    Voc = Vo;
  }
  float dustDensity = dV / K * 170.0;
  //Serial.println(dustDensity);
  return dustDensity;

}

// function
String getHumidity(String x){
  String ans = "";
    String ans2 = "";
    String ans3 = "";
    int cnt = 0;

    int idx = 0;
    while(x[idx] < '0' || x[idx] > '9' ){
        idx++;
    }

    while((x[idx] >= '0' && x[idx] <= '9') || x[idx] == '.' || x[idx] == '-'){
        ans += x[idx];
        idx++;
    }
    while(x[idx] < '0' || x[idx] > '9' ){
        idx++;
    }
    while((x[idx] >= '0' && x[idx] <= '9') || x[idx] == '.' || x[idx] == '-'){
        ans3 += x[idx];
        idx++;
    }

    return ans;

}

String getTemp(String x){
  String ans = "";
    String ans2 = "";
    String ans3 = "";
    int cnt = 0;

    int idx = 0;
    while(x[idx] < '0' || x[idx] > '9' ){
        idx++;
    }

    while((x[idx] >= '0' && x[idx] <= '9') || x[idx] == '.' || x[idx] == '-'){
        ans += x[idx];
        idx++;
    }
    while(x[idx] < '0' || x[idx] > '9' ){
        idx++;
    }
    while((x[idx] >= '0' && x[idx] <= '9') || x[idx] == '.' || x[idx] == '-'){
        ans2 += x[idx];
        idx++;
    }
    // while(x[idx] < '0' || x[idx] > '9' ){
    //     idx++;
    // }
    // while((x[idx] >= '0' && x[idx] <= '9') || x[idx] == '.' || x[idx] == '-'){
    //     ans3 += x[idx];
    //     idx++;
    // }

    return ans2;

}

// String getDust(String x){
//   String ans = "";
//     String ans2 = "";
//     String ans3 = "";
//     int cnt = 0;

//     int idx = 0;
//     while(x[idx] < '0' || x[idx] > '9' ){
//         idx++;
//     }

//     while((x[idx] >= '0' && x[idx] <= '9') || x[idx] == '.' || x[idx] == '-'){
//         ans += x[idx];
//         idx++;
//     }
//     while(x[idx] < '0' || x[idx] > '9' ){
//         idx++;
//     }
//     while((x[idx] >= '0' && x[idx] <= '9') || x[idx] == '.' || x[idx] == '-'){
//         ans2 += x[idx];
//         idx++;
//     }
//     while(x[idx] < '0' || x[idx] > '9' ){
//         idx++;
//     }
//     while((x[idx] >= '0' && x[idx] <= '9') || x[idx] == '.' || x[idx] == '-'){
//         ans3 += x[idx];
//         idx++;
//     }

//     return ans3;

// }

void loop() {
      Serial.println("Connecting Comm ...");  
      yield();

      String Data1,Data2,Data3,Data4;

      //connect to stm
      if(comm.available()){
        Serial.println("Connecting Comm2 ...");
        Data1 = comm.readStringUntil('{');
        Data2 = comm.readStringUntil(',');
        Data3 = comm.readStringUntil(',');
        Data4 = comm.readStringUntil('}');

        Data2.replace(" ","");
        Data3.replace(" ","");
        Data4.replace(" ","");

        Serial.println(Data1);
        Serial.println("s");
        Serial.println(Data2);
        Serial.println(Data3);
        Serial.println(Data4);

        //humidityData = getHumidity(Data1).toFloat();
        //tempData = getTemp(Data1).toFloat();
        // dustData = getDust(Data1).toFloat();
        //dustData = DustZ();
        humidityData = Data2;
        tempData = Data3;
        dustData = Data4;

        Serial.println(humidityData);
        Serial.println(tempData);
        Serial.println(dustData);

        yield();
        
        if (Firebase.ready()) {
          Serial.println("can update in firebase");

          json.set(humidityPath, humidityData.toFloat());
          json.set(tempPath, tempData.toFloat());
          json.set(dustPath, dustData);
          Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, "/data", &json) ? "ok" : fbdo.errorReason().c_str());
          // humidity = humidity + 2;
          // temp = temp + 5;
          // dust = dust + 1;
        }
        else {
          Serial.println("chip hai laewwwwww cannot update");
          
        }

      }else{
        Serial.println("comm not available");
      }

      //connect to firebase
      
  
  delay(1000);
}