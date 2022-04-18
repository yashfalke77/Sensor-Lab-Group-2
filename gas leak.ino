# define GREEN 2
# define ORANGE 3
# define RED 4
# define Buzzer 5
# define Sensor A0
# define Sensor A1

String ssid     = "";  // SSID to connect to
String password = ""; // Our virtual wifi has no password 
String host     = "api.thingspeak.com"; // Open Weather Map API
const int httpPort   = 80;
String uri     = "/update?api_key=B4GX97D82G7OFYH1&field1=";

int setupESP8266(void) {
  // Start our ESP8266 Serial Communication
  Serial.begin(115200);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 1;
    
  // Connect to 123D Circuits Simulator Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 2;
  
  // Open TCP connection to the host:
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 3;
  
  return 0;
}

void anydata(void) {
  
  int sensor_In = analogRead(A0);
  int tmp = analogRead(A1);
  int celsius = map(((tmp - 20) * 3.04), 0, 1023, -40, 125);
  // Construct our HTTP call
  String httpPacket = "GET " + uri + String(sensor_In) + "&field2=" + String(celsius) + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();
  
  // Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10); // Wait a little for the ESP to respond if (!Serial.find(">")) return -1;

  // Send our http request
  Serial.print(httpPacket);
  delay(10); // Wait a little for the ESP to respond
  if (!Serial.find("SEND OK\r\n")) return;
   
}


void status_Indicator()
{
int sensor_In = analogRead(A0);
int tmp = analogRead(A1);
int temperatureC = map(((tmp - 20) * 3.04), 0, 1023, -40, 125);

Serial.print("Gas Readings: ");
Serial.println(sensor_In);
Serial.print("Temp Readings: ");
Serial.println(temperatureC);

if ( sensor_In >=14 || temperatureC >= 40 )
  {
  digitalWrite(RED, HIGH);
  digitalWrite(Buzzer,HIGH);
  }
else if (sensor_In >=8 || temperatureC >= 45)
  {
  digitalWrite(ORANGE, HIGH);
  } else  
  {
  digitalWrite(GREEN, HIGH);
  }
delay(1000);
}


void setup() {
  for (int i = 2; i < 6; i++)
	  pinMode(i, OUTPUT);
  
  Serial.begin(9600);
  // setupESP8266();
               
}


void loop() {
 for (int i = 2; i < 6; i++)
	digitalWrite(i, LOW);

 status_Indicator();
//  anydata();
  
}