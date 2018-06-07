/*
 Created by Rui Santos
 Visit: http://randomnerdtutorials.com for more arduino projects

 Arduino with Ethernet Shield
 */
#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h> 
#include <Bridge.h>
#include <HttpClient.h>
//#include <ICMPPing.h>



 signed long next;


//#include <EEPROM.h>
const int test= 1;
const int gate1 = 4;  // front gate
const int gate2 = 6;  // portico door
const int buttonPin = 2;
int gate2state=0;
int first=0;
int last=90;
Servo myservo;
int val;
int pos=90 ; 
unsigned long  cntNum=50000;
unsigned long cnt;
int cnt2;
int buttonState = 0;

int serveState=0;
int serveMillis=0;
int i=0;

unsigned long difMillis=0;

// defined TEST
  //byte mac[] = { 0x00, 0xAD, 0xBA, 0xEF, 0xFE, 0x00 };   //physical mac address  *** Test : 00  real : EA
  //IPAddress ip(192,168,0,44);                           // ip in lan   *** Test : 44  real : 33
// defined REAL
   byte mac[] = { 0x00, 0xAD, 0xBA, 0xEF, 0xFE, 0xEA };   //physical mac address  *** Test : 00  real : EA
  IPAddress ip(192,168,0,33);                           // ip in lan   *** Test : 44  real : 33


IPAddress gateway(192,168,0,1); // internet access via router
IPAddress subnet(255,255,255,0); //subnet mask
EthernetServer server(80);                             //server port     
String readString;
IPAddress myserver(64,233,187,99 ); // Google
//void gateOpen();
//void doorClose();
//void doorOpen();





//EthernetClient client;

//byte clientServer[] = { 64, 233, 187, 99 }; // Google


void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
 // }
  pinMode(gate1, OUTPUT);
  pinMode(gate2, OUTPUT);
  pinMode(buttonPin, INPUT);
  //pinMode(2, INPUT_PULLUP);
  Serial.println("test1");
  myservo.attach(5);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, subnet,gateway);
  server.begin();
  Serial.println(Ethernet.localIP());
  cnt=0;
  //clientCon();


  
}

void(* resetFunc)(void)=0;

void loop() {


  if(serveMillis<2000)
  {
    serveMillis++;
    delay(10);
  }else
  {
    serveMillis=0;
    Ethernet.begin(mac, ip, subnet,gateway);
    server.begin();
  }



  // Create a client connection
  //int sensorVal1 = digitalRead(2);
  buttonState = digitalRead(buttonPin);

  
  if(gate2state==1)cnt++;
  if(cnt>cntNum)    //100,000  :  5second
  {
    cnt2++;
    cnt=0;
    if(cnt2>70)
    {
      doorClose();
    }
    //Serial.println(cnt2);
    //Serial.println();
    delay(10);
  }
  if(buttonState==HIGH )
  {
    
    difMillis++;
    delay(10);
  } else
  {
    difMillis=0;
  }
  
  if (buttonState == HIGH && difMillis >2) {
    if(gate2state==1)
    {
      doorClose();
      delay(10);
      Serial.println("door close");
    } else
    {
      doorOpen();
      delay(10);
      Serial.println("door open");
    }
    difMillis=0;
  }
  
  EthernetClient client = server.available();


 
  if (client) {
    
    while (client.connected()) {   
      if (client.available()) {
        //Serial.println("(client.available()"); //print to serial monitor for debuging
        char c = client.read();
        
        //read char by char HTTP request
        
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
         }

         //if HTTP request has ended
         if (c == '\n') {          
           Serial.println(readString); //print to serial monitor for debuging
     
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
           client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           client.println("<meta name ='viewport' content ='initial-scale = 1.0, user-scalable = no'>");

          client.println("<style>");
          client.println("#container{");
          client.println("width: 300px;");
          client.println("margin: 0 auto;}");
          client.println("li {list-style-type: none; }");
          client.println("</style>");
          
           client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
           client.println("<TITLE>Relay Project</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<H1>MEISTER PROJECT</H1>");
           client.println("<hr />");
           client.println("<br />");  
           client.println("<H2>DM home Ethernet system</H2>");
           
           client.println("<br />");  
           client.println("<a href=\"/?gate1on\"\" >Open Gate1 and door</a>");
           client.println("<br />");   
           client.println("<br />"); 
           client.println("<br />");  
           client.println("<a href=\"/?gate1OnlyOn\"\" >Gate1 </a>");
           client.println("<br />");   
           client.println("<br />"); 
           client.println("<br />"); 
           client.println("<a href=\"/?door\"\" > Door</a>");

           client.println("<br />");  
           client.println(" <?php echo 'test'; ?>");


           client.println("<br />"); 
           client.println("<br />"); 
          // client.println("<a href='' onclick="<script > window.open('', '_self', ''); window.close();</script>" >  Close Page </a>");
           client.println("<br />"); 
           client.println("</BODY>");
           client.println("</HTML>");
         if (readString.indexOf("?gate1on") >0)
          {
                client.println("<script > window.open('', '_self', ''); window.close(); </script>");
           }
           delay(10);
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons
           if (readString.indexOf("?gate1on") >0)
           {
              gateOpen();
                
             if(gate2state==0)
             {
                doorOpen();
             }
           }
           if (readString.indexOf("?gate1OnlyOn") >0)
           {
                gateOpen();
           }
           if (readString.indexOf("?door") >0)
           {
                if(gate2state==0)
                {
                  doorOpen();
                }
                else
                {
                  doorClose();
                }
                
           }
            //clearing string for next read
          readString="";
         }
       }
    }
}


}



void gateOpen()
{
  digitalWrite(gate1, HIGH);
  delay(80); 
  digitalWrite(gate1, LOW);
  delay(1000);
}
void doorOpen()
{
  digitalWrite(gate2, HIGH);
  delay(300); 
  for (pos=first ; pos <= last; pos+=1) { // goes from 0 degrees to 180 degrees
                  // in steps of 1 degree
                  //posR=pow(2,pos);
  myservo.write(pos);              // tell servo to go to position in variable 'pos'
  delay(15); 
  }
  for (i=0;i<10;i++)
  {
    myservo.write(last);              // tell servo to go to position in variable 'pos'
    delay(15); 
  }
  gate2state=1;
  cnt=0;
  cnt2=0;
  delay(100); 
  digitalWrite(gate2, LOW);
}

void doorClose()
{
  digitalWrite(gate2, HIGH);
    delay(300); 
    for (pos=last ; pos >=first; pos-=1) { // goes from 0 degrees to 180 degrees
                  // in steps of 1 degree
      
     myservo.write(pos);              // tell servo to go to position in variable 'pos'
     delay(15);                       // waits 15ms for the servo to reach the position
     }
   for (i=0;i<10;i++)
  {
    myservo.write(first);              // tell servo to go to position in variable 'pos'
    delay(15); 
  }
     gate2state=0;
     cnt=0;
     cnt2=0;
     delay(100);
     digitalWrite(gate2, LOW);
     Serial.println("door close");
}


