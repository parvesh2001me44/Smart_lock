# Smart-Lock-Prototype

<img src="/images/circuit.jpg" height="250" >
<br  />

📷 About Project -  

🚩   In this project we've build IoT based smart lock which have been accessed through RFID.  

🚩   So now, if user want to unlock this lock, then he or she should have Valid RFID.  

🚩   Also we have programmed this lock system to keep record data of user entries with user name and timestamp. This data will recorded on our dedicated my S.Q.L database which is hosted on our webserver, hence the system Administrator can login to that web portal and, keep track of how many time this lock got opened.  


<br  />

📜 Parts Required  

1. NodeMCU ESP32 Development Board  
2. RC522 RFID Reader Module  
3. MB102 Breadboard Power Supply module  
6. 12V Solenoid Lock  
7. 2 Channel Relay Board 5V    
8. Batteries - 2*9V LiPo and Power Bank.  

<br  />

Libraries Used  
  
📕  MFRC RC5222 RFID Library  -  Arduino IDE Library Manager  

📕  NTP Client Library  - Arduino IDE Library Manager  

📕  SPI library - Library for RC522 RFID reader

📕  HTTP Client Library - Required for HTTP POST/GET request, functions
  
