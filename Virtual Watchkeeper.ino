#include <common.h>
#include <FirebaseESP8266.h>
#include <FirebaseFS.h>
#include <Utils.h>
#include <SPI.h>
#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>
#define FIREBASE_HOST "dukh2-49941-default-rtdb.firebaseio.com"                   
#define FIREBASE_AUTH "jlgmWCQ0AYga3Dh3k9M0rVg5b1mPpNBzNigfJgoc"

#define WIFI_SSID "Note 5"
#define WIFI_PASSWORD "KARAN123"
FirebaseData fbdo;
FirebaseData kbdo;

#define Relay1 12 //D6
int val1;
int ledpin = 4;
int pirpin = 5;
int pirstate = LOW;
int val = 0;

#define SMTP_HOST "smtp.gmail.com"


#define SMTP_PORT 465

#define AUTHOR_EMAIL "prernadalwani18@gmail.com"
#define AUTHOR_PASSWORD "prerna@181403"
String val2;

SMTPSession smtp;

void smtpCallback(SMTP_Status status);

void setup()
{

  Serial.begin(115200);

#if defined(ARDUINO_ARCH_SAMD)
  while (!Serial);
    
  Serial.println();

#endif

  Serial.println();

  Serial.print("Connecting to AP");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.setString(fbdo,"S16", "0");


}
   void firebasereconnect()
{
  Serial.println("Trying to reconnect");
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  }
void loop()
{
   /* if (Firebase.failed())
      {
      Serial.print("setting number failed:");
      Serial.println(Firebase.error());
      firebasereconnect();
      return;
      }*/
      
  Firebase.getInt(fbdo,"S16");
  val1 =  fbdo.stringData().toInt();
 
  if(val1==1)                     
 {
  val = digitalRead(pirpin);
  Serial.println(val);
  if(val == HIGH){


    digitalWrite(ledpin,HIGH);
    if (pirstate == LOW){
      Serial.println("Motion Detected");
      pirstate =HIGH;
     smtp.debug(1);

  
  smtp.callback(smtpCallback);

  
  ESP_Mail_Session session;

  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "mydomain.net";


  SMTP_Message message;

  Firebase.getString(kbdo,"S22");
  val2 = kbdo.stringData();
  val2 = val2.substring(2,val2.length()-2);
  message.sender.name = "WorryFree";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Intruder Alert";
  message.addRecipient("Mummy", "vinitadalwaniinspire@gmail.com");
  message.addRecipient("Me", "karand.it@sbjit.edu.in"); 
  message.addRecipient("Pranay", "pranaym.it@sbjit.edu.in"); 
  message.addRecipient("Nisha", "nishaa.it@sbjit.edu.in"); 
  message.addRecipient("Nileema", "nileemap.it@sbjit.edu.in");
  message.addRecipient("User", val2.c_str());    

  String textMsg = "Someone has breaked into your house.";
  message.text.content = textMsg.c_str();



  if (!smtp.connect(&session))
    return;

  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
    
  ESP_MAIL_PRINTF("Free Heap: %d\n", MailClient.getFreeHeap());
      delay(3000);
      }

  }
else{
  digitalWrite(ledpin,LOW);
  if(pirstate == HIGH){
    Serial.println("motion Detected email sent");
    pirstate = LOW;
  }
}
}
}
 void smtpCallback(SMTP_Status status)
{

  Serial.println(status.info());


  if (status.success())
  {
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    
    Serial.println("----------------\n");
  }
}
