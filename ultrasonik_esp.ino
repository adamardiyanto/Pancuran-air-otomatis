#include "WiFiEsp.h"
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2,3);
#define trigger 9 
#define echo 8
#define pump 12

long durasi = 0, jarak = 0;
char ssid[] = "Redmi Note 9";        // Isi dengan nama profil Wifi
char pass[] = "tanyaapa";            // password wifi
char server[] = "192.168.43.91";     // alamat access point yang telah terinstall XAMPP local host
//char server[] = "semesin.com";     // alamat web hosting
 
//float namaVariabel[] = "Variabel";
String text = "";
String Respon = "";
bool responDariServer = false;
 
bool statusKomunikasiWifi = false;
long waktuMulai;
long waktuMintaData = 5000; 

int debit=200; 
WiFiEspClient client;
int status = WL_IDLE_STATUS;
 
void setup()
{
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(pump,OUTPUT);
  
  Serial.begin(115200);
  Serial.println("Koneksi arduino dengan mySql menggunakan ESp8266 dan XAMPP");
  Serial.println("Ketik pesan yang akan dikirim (pastikan setting serial ke \"both NL & CR\")");
  Serial.println("http://www.semesin.com/project");
  Serial.println();
 
  Serial1.begin(9600);
  WiFi.init(&Serial1);
 
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
 
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
 
  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
   
  printWifiStatus();
  waktuMulai = millis();
}
 
void loop()
{
  //tunggu imputan nilai dari untuk dikirim ke server
  
    ukur_jarak();
 if(jarak<10){
  digitalWrite(pump,HIGH);
  
  //delay(5000);
 }else{
  digitalWrite(pump,LOW);
  delay(1000);
 }

 
    if(digitalRead(pump)==HIGH)
    {
      statusKomunikasiWifi = kirimKeDatabase(debit);
      
      delay(1000);
    }
  
 

  if(statusKomunikasiWifi)
  {
    // if there are incoming bytes available
    // from the server, read them and print them
    while (client.available()) 
    {
      char c = client.read();
      Respon += c;
    }
   
    // if the server's disconnected, stop the client
    if (!client.connected()) {
      Serial.println("Disconnecting from server...");
      client.stop();
      statusKomunikasiWifi = false;
      responDariServer = true;
    }
  }
 
  

}




 
bool kirimKeDatabase(int namaVariabel)
{
  Serial.println();
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request
 
    // parameter 1
    client.print("GET /IOT/input.php?");
    client.print("variabel=");
    client.print(namaVariabel);
     
   
     
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
 
    return true;
  }
  return false;
}
 
void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
 
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
 
  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
 
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("gateway:");
  Serial.print(gateway);
  Serial.println(" ");
}

void ukur_jarak(){
  
 digitalWrite(trigger, HIGH);
delayMicroseconds(10);
digitalWrite(trigger, LOW);
durasi = pulseIn(echo, HIGH);
jarak = (durasi * 0.034)/2;

}
