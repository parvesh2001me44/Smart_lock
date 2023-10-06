#include <WiFi.h>                   // include wifi related libraries
#include <NTPClient.h>              // Required for fetching current time
#include <HTTPClient.h>             // Required for HTTP POST/GET request, functions

#include <SPI.h>                    // Library required for RC522 RFID Reader
#include <MFRC522.h>                // Library of RC522 RFID Reader

#define SS_PIN 21                   // SDA/SS Pin of RFID Reader
#define RST_PIN 22                  // RESET Pin of RFID Reader
#define LockerPin 14    
#define  solenoidPin 12          // Relay driving Solenoid lock is connected to this pin (Active Low)

// Replace with your network credentials
const char* ssid = "*********";
const char* password = "**********";

// Website Details
// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "****************";

// Keep this API Key value to be compatible with the PHP code provided in the Github. 
// If you change the apiKeyValue value, the PHP file /post-data.php also needs to have the same key 
String apiKeyValue = "***********";

MFRC522 mfrc522(SS_PIN, RST_PIN);            // Create MFRC522 instance.
String user="user2";                              // String variable For Username
String data="";                              // String variable For PIN data from Nextion
String branchName = "SecureCoBank";
String userName ="abhi";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");


String RealT = "";
//Week Days
String weekDays[7]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

//Month names
String months[12]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};



char buffer[10] = {0};   // Buffer for collecting text values

//************send data to the website*************
void SendDataToWebsite(){
  //Check WiFi connection status
  Serial.println("Sending Data to Website");
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    
    HTTPClient http;    //Declare object of class HTTPClient
    
    GetDateTime();
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&userID=" + user  + "&userName=" + userName + "&branchName=" + branchName + "";
                          
                          
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
    
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}
//*****************send data to the website*********

void initRFID(){
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
}

void OpenRFID(){

    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
      return;
    }

    String content= "";
    user="";
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    content.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading

    if (content.substring(1) == "60 32 49 1E") //change here the UID of the card/cards that you want to give access
    {
      digitalWrite(LockerPin, LOW); // Activate the relay
      digitalWrite(solenoidPin, HIGH);
      user = "user2";
      Serial.println("user Identified");
      // ValidRFID.show();
      GetDateTime();
      SendDataToWebsite();

      delay(3000);

    }

}
void CloseRFID(){

    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
      return;
    }

    String content= "";
    user="";
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    content.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading

    if (content.substring(1) == "60 32 49 1E") //change here the UID of the card/cards that you want to give access
    {
      pinMode(LockerPin, OUTPUT);  
      pinMode(solenoidPin, OUTPUT);         // Seting Relay PIN as Output
      digitalWrite(LockerPin, HIGH);     
      digitalWrite(solenoidPin, LOW); 
      user = "user2";
      Serial.println("user Identified");

      
      GetDateTime();
      SendDataToWebsite();
      
      delay(3000);
      
    }

}


// Connect to Wi-Fi
void connectWifi(){
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

//Init NTPclient to get time
void initNTP(){
  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 1 * 60 * 60 = 3600
  // GMT +5 = 5 * 60 * 60 = 18000
  // GMT -1 = -1 *60 * 60 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(19080);
}

/* Function which sets current date time */

void GetDateTime(){
  timeClient.update();

  unsigned long epochTime = timeClient.getEpochTime();
  
  String formattedTime = timeClient.getFormattedTime();

  String weekDay = weekDays[timeClient.getDay()];

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;

  int currentMonth = ptm->tm_mon+1;

  String currentMonthName = months[currentMonth-1];

  int currentYear = ptm->tm_year+1900;

  RealT = weekDay + " " + currentMonthName + " " + String(monthDay) + " " + formattedTime + " " + String(currentYear);

  //Serial.println(RealT);
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  Serial.println("IoT based Smart Lock V2.0");
  delay(1000);

  /* Initialize the display and other peripherals */
  Serial.println("Setting up WiFi");
  connectWifi();
  delay(500);

  Serial.println("Fetching Time");
  initNTP();
  delay(250);
 
  pinMode(LockerPin, OUTPUT);  
  pinMode(solenoidPin, OUTPUT);         // Seting Relay PIN as Output
  digitalWrite(LockerPin, HIGH);     
  digitalWrite(solenoidPin, LOW);     
     // Relay PIN is active LOW

  Serial.println("Setting up RFID");
  initRFID();
  delay(500);

  GetDateTime();
  delay(1000);
}

void loop() {
    /*
     * When a pop or push event occured every time, 
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
  // nexLoop(nex_listen_list);
  OpenRFID();
  // SendDataToWebsite();
  CloseRFID();
  // SendDataToWebsite();
}
/*======================================= ===============================================================*/
