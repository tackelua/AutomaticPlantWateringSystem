// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       NodeControlMrTinh.ino
    Created:	7/12/2018 8:54:15 AM
    Author:     DESKTOP-PVDO71Q\tdhie
	Version:	0.3
*/

// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>


//data Json
#define SPRAY			"S"
#define S_HUMI			"H"
#define ON				"T"
#define OFF				"F"

//Pin In Out
#define PLANT1_SPRAY	2
#define PLANT1_S_HUMI	A0

#define PLANT2_SPRAY	3
#define PLANT2_S_HUMI	A1

#define PLANT3_SPRAY	4
#define PLANT3_S_HUMI	A2

#define PLANT4_SPRAY	5
#define PLANT4_S_HUMI	A3

#define PLANT5_SPRAY	6
#define PLANT5_S_HUMI	A4

#define PLANT6_SPRAY	7
#define PLANT6_S_HUMI	A5

#define PLANT7_SPRAY	8
#define PLANT7_S_HUMI	A6

#define PLANT8_SPRAY	9
#define PLANT8_S_HUMI	A7

#define In(pin)				pinMode(pin, INPUT)
#define Ou(pin)				pinMode(pin, OUTPUT)
#define Read_Humi(pin)		analogRead(pin)
#define ON_Spray(pin)		digitalWrite(pin, HIGH)
#define	OFF_Spray(pin)		digitalWrite(pin, LOW)
// Data val
enum Plants
{
	plant_1, plant_2, plant_3, plant_4, plant_5, plant_6, plant_7, plant_8
};

bool	SttSpray[8];
int		S_huminity[8];


// Define Functions below here or use other .ino or cpp files
//
void ConfigIO() {
	for (uint8_t i = 0; i <= 7; i++) {
		In(i + PLANT1_S_HUMI);
		Ou(i + PLANT1_SPRAY);
		OFF_Spray(i + PLANT1_SPRAY);
	}
}
void ReadSS(uint32_t time_s_Refresh) {
	uint32_t time_refresh = time_s_Refresh * 1000;
	static unsigned long timeUD = millis();
	if (millis() - timeUD - 100 > time_refresh) {
		timeUD = millis();
		for (uint8_t i = plant_1; i <= plant_8; i++) {
			S_huminity[i] = Read_Humi(i + PLANT1_S_HUMI);
			S_huminity[i] = map(S_huminity[i], 0, 1023, 100, 0);
		}
	}
}
void Upload(uint32_t time_s_Refresh) {
	uint32_t time_refresh = time_s_Refresh * 1000;
	static unsigned long timeUD = millis();
	if (millis() - timeUD + 100> time_refresh) {
		timeUD = millis();
		UpData();
	}
}
void UpData() {
	DynamicJsonBuffer JsBufUpdate(256);
	JsonObject& JsUpdate = JsBufUpdate.createObject();
	for (uint8_t i = plant_1; i <= plant_8; i++) {
		JsUpdate[S_HUMI + String(i)] = S_huminity[i];
		JsUpdate[SPRAY + String(i)] = SttSpray[i] == true ? ON : OFF;
	}
	String UpdateData;
	JsUpdate.printTo(UpdateData);
	if (Serial.available() <= 0) {
		Serial.println(UpdateData);
	}
}
void Control() {
	if (Serial.available()) {
		static String Messenger = "";
		Messenger.reserve(256);
		Messenger = Serial.readString();

		DynamicJsonBuffer JsBufRecevie(256);
		JsonObject& JsRecevie = JsBufRecevie.parseObject(Messenger);
		if (!JsRecevie.success())
		{
			return;
		}
		for (uint8_t i = plant_1; i <= plant_8; i++) {
			String sprayRece = JsRecevie[SPRAY + String(i)];
			sprayRece == ON ? SttSpray[i] = true : SttSpray[i] = false;
			digitalWrite(PLANT1_SPRAY + i, !SttSpray[i]);
		}
		UpData();
	}
}
// The setup() function runs once each time the micro-controller starts
void setup()
{
	Serial.begin(115200);
	ConfigIO();
	delay(500);
	ReadSS(0); 
	UpData();
}

// Add the main program code into the continuous loop() function
void loop()
{
	Control();
	ReadSS(2);
	Upload(2);
}
