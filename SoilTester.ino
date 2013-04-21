#include <SerialCommand.h>
#include <dht11.h>
int ledpin = 13; // Onboard LED
double moisture = 0.0;

dht11 DHT11;
SerialCommand sCmd;  
#define DHT11PIN 2


void OUTPUT_INFO() {
  
  Serial.print("{\"id\": \"182791\",\"time\": \"13:56:13 20/04/2013\",\"lat\": \"97.03125\",\"long\": \"107.2890\",\"hasAltitude\":true,\"altitude\": \"10\",\"hasPH\":true,\"pH\":\"7\",\"hasMoisture\":true,\"moisture\":\"");
  Serial.print(moisture);
  Serial.print("\",\"hasTemperature\":true,\"temperature\":\"");
  Serial.print((float)DHT11.temperature, 2);
  Serial.println("\"}");
  
  //Serial.print(",");
  //Serial.print((float)DHT11.humidity, 2);
  ///Serial.print(",");
  //Serial.print(dewPoint(DHT11.temperature, DHT11.humidity));
  //Serial.print(",");

  //Serial.println("]");
  
  //digitalWrite(ledpin, HIGH);
  //delay(3000);
  //digitalWrite(ledpin, LOW);
  
 // Serial.print("Read sensor: ");
 // Serial.println("\n");
 // Serial.print("Humidity (%): ");
 // Serial.println((float)DHT11.humidity, 2);

 // Serial.print("Temperature (oC): ");
 // Serial.println((float)DHT11.temperature, 2);

//  Serial.print("Temperature (oF): ");
 // Serial.println(Fahrenheit(DHT11.temperature), 2);

//  Serial.print("Temperature (K): ");
//  Serial.println(Kelvin(DHT11.temperature), 2);

 // Serial.print("Dew Point (oC): ");
 // Serial.println(dewPoint(DHT11.temperature, DHT11.humidity));

 // Serial.print("Dew PointFast (oC): ");
 // Serial.println(dewPointFast(DHT11.temperature, DHT11.humidity));
  
 // Serial.print("Moisture (resistance): ");
 // Serial.println(moisture);
}



void setup()
{
  pinMode(ledpin, OUTPUT);  // Onboard LED as OUTPUT
  digitalWrite(ledpin, LOW);
  
  Serial.begin(9600);
  Serial.println("Soil Probe v1.0");
  
  sCmd.addCommand("[out]", OUTPUT_INFO);
}

void loop()
{
   sCmd.readSerial();
  
  moisture = analogRead(0);
  
  int chk = DHT11.read(DHT11PIN);

  switch (chk)
  {
    case DHTLIB_OK: 
		//Serial.println("OK"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.println("Checksum error"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.println("Time out error"); 
		break;
    default: 
		Serial.println("Unknown error"); 
		break;
  }

//  OUTPUT_INFO();

  delay(1000);
}



// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm 
double dewPoint(double celsius, double humidity)
{
	double RATIO = 373.15 / (273.15 + celsius);  // RATIO was originally named A0, possibly confusing in Arduino context
	double SUM = -7.90298 * (RATIO - 1);
	SUM += 5.02808 * log10(RATIO);
	SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
	SUM += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
	SUM += log10(1013.246);
	double VP = pow(10, SUM - 3) * humidity;
	double T = log(VP/0.61078);   // temp var
	return (241.88 * T) / (17.558 - T);
}

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
	double a = 17.271;
	double b = 237.7;
	double temp = (a * celsius) / (b + celsius) + log(humidity/100);
	double Td = (b * temp) / (a - temp);
	return Td;
}


