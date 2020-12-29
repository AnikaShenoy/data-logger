#include <SoftwareSerial.h>
String ssid="yourSSID";
String password="yourPassword";
SoftwareSerial esp(6,7);//RX, TX (pin to receive, pin to transmit)
String data;
String server = "www.blahblah.com";
String uri = "/esppost.php" //substitute with our actual uri
int DHpin = 8;//sensor pin
byte dat[5];
String data1, data2;

void setup() {
  pinMode (DHpin, OUTPUT);
  esp.begin(9600);
  Serial.begin(9600); //baud rate
  reset();
  connectWifi();
}

void connectWifi(){
  String cmd = "AT+CWJAP=\"" + sside + "\",\"" + password + "\""; //AT Commands 
  esp.println(cmd);
  delay(4000);
  if (esp.find("OK")){
    Serial.println("Connected")
  }
  else {
    connectWifi();
    Serial.println("Cannot connect to Wifi");
  }
}

byte read_data(){
  byte data;
  for (int i = 0; i < 8; i++){
    if digitalRead(DHpin) == LOW){
      while (digitalRead(DHpin) == LOW); //wait for 50 us
        delayMicroseconds(30);
    }
    if digitalRead(DHpin) == HIGH
      data|= (1 << 7-i));
    while (digitalRead(DHpin) == HIGH);
  }
  return data
}

void start_test(){
  digitalWrite(DHpin, LOW);
  delay(30);
  digitalWrite(DHpin, HIGH);
  delayMicroseconds(40);
  pinMode(DHpin, INPUT);
  while (digitalRead(DHpin) == HIGH);
    delayMicroseconds(80);
  if digitalRead(DHpin)==LOW
    delayMicroseconds(80)
  for (int i = 0; i<4; i++)
    dat[i] = read_data();
    pinMode(DHpin, OUTPUT);
    digitalWrite(DHpin, HIGH);
}

void loop() {
  start_test();
  data1 = String(dat[0]);
  data2 = String(dat[1]);
  data = "data1= " + data1 + "&data2= " + data2;
  sendMessageToComputer();
  delay(1000);
}

void sendMessageToComputer(){
  esp.println("AT+CIPSTART=\"TCP\","\"" + server + "\",80");
  if (esp.find("OK")){
    Serial.println("TCP connection ready");
  } delay(1000);
  String postRequest = 
  "POST" + url + "HTTP/1.0\r\n" + 
  "Host:" + server + "\r\n" + 
  "Accept:*" + "/" + "*\r\n" + 
  "Content-Length:" + data.length() + "\r\n" + 
  "Content-Type:application/x-www-form-urlencoded\r\n" + 
  "\r\n" + data;
  String sendCmd = "AT+CIPSEND=";//determine number of characters to be sent
  esp.print(sendCmd);
  esp.println(postRequest.length()); 
  delay(500);
  if (esp.find(">")){
    Serial.println("Sending..");
    esp.print(postRequest);
    if esp.find("SEND OK")){
      Serial.println("Packet sent");
      while (esp.available()){
        String tmpResp = esp.readString();
        Serial.println(tmpResp);
      }
    esp.println("AT+CIPCLOSE");
    }
  }
}
