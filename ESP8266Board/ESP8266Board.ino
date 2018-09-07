// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       HubControlTinh.ino
    Created:	7/13/2018 8:43:13 AM
    Author:     DESKTOP-PVDO71Q\tdhie
*/

// Define User Types below here or use a .h file
//



// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// The setup() function runs once each time the micro-controller starts
//Include
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WidgetRTC.h>

//Setup Blynk
#define HieuTest

#ifdef HieuTest
char auth[] = "7a638e14f84d4a04b7702882d737aec5";
#else

#endif
#define TERMINAL				V0

#define WATER_LEVEL				V10
#define DISPLAY_MAIN			V11
#define	BUTTON_PUM_MAIN			V12		
#define	BUTTON_AUTO				V13

#define NAME_PLANTS1			V20
#define	DISPLAY_PLANTS1			V21
#define	BUTTON_PUM_PLANTS1		V22
#define	BUTTON_AUTO_PLANTS1		V23
#define	HIGHER_LIMIT_PLANTS1		V24
#define	LOWER_LIMIT_PLANTS1		V25
#define	CHART_HUMI_PLANTS1		V26
#define	CHART_SPRAY_PLANTS1		V23
#define	LED_STATUS_PLANTS1		V27

#define NAME_PLANTS2			V30
#define	DISPLAY_PLANTS2			V31
#define	BUTTON_PUM_PLANTS2		V32
#define	BUTTON_AUTO_PLANTS2		V33
#define	HIGHER_LIMIT_PLANTS2		V34
#define	LOWER_LIMIT_PLANTS2		V35
#define	CHART_HUMI_PLANTS2		V36
#define	CHART_SPRAY_PLANTS2		V33
#define	LED_STATUS_PLANTS2		V37

#define NAME_PLANTS3			V40
#define	DISPLAY_PLANTS3			V41
#define	BUTTON_PUM_PLANTS3		V42
#define	BUTTON_AUTO_PLANTS3		V43
#define	HIGHER_LIMIT_PLANTS3		V44
#define	LOWER_LIMIT_PLANTS3		V45
#define	CHART_HUMI_PLANTS3		V46
#define	CHART_SPRAY_PLANTS3		V43
#define	LED_STATUS_PLANTS3		V47

#define NAME_PLANTS4			V50
#define	DISPLAY_PLANTS4			V51
#define	BUTTON_PUM_PLANTS4		V52
#define	BUTTON_AUTO_PLANTS4		V53
#define	HIGHER_LIMIT_PLANTS4		V54
#define	LOWER_LIMIT_PLANTS4		V55
#define	CHART_HUMI_PLANTS4		V56
#define	CHART_SPRAY_PLANTS4		V53
#define	LED_STATUS_PLANTS4		V57

#define NAME_PLANTS5			V60
#define	DISPLAY_PLANTS5			V61
#define	BUTTON_PUM_PLANTS5		V62
#define	BUTTON_AUTO_PLANTS5		V63
#define	HIGHER_LIMIT_PLANTS5		V64
#define	LOWER_LIMIT_PLANTS5		V65
#define	CHART_HUMI_PLANTS5		V66
#define	CHART_SPRAY_PLANTS5		V63
#define	LED_STATUS_PLANTS5		V67

#define NAME_PLANTS6			V70
#define	DISPLAY_PLANTS6			V71
#define	BUTTON_PUM_PLANTS6		V72
#define	BUTTON_AUTO_PLANTS6		V73
#define	HIGHER_LIMIT_PLANTS6		V74
#define	LOWER_LIMIT_PLANTS6		V75
#define	CHART_HUMI_PLANTS6		V76
#define	CHART_SPRAY_PLANTS6		V73
#define	LED_STATUS_PLANTS6		V77

#define NAME_PLANTS7			V80
#define	DISPLAY_PLANTS7			V81
#define	BUTTON_PUM_PLANTS7		V82
#define	BUTTON_AUTO_PLANTS7		V83
#define	HIGHER_LIMIT_PLANTS7		V84
#define	LOWER_LIMIT_PLANTS7		V85
#define	CHART_HUMI_PLANTS7		V86
#define	CHART_SPRAY_PLANTS7		V83
#define	LED_STATUS_PLANTS7		V87

#define NAME_PLANTS8			V90
#define	DISPLAY_PLANTS8			V91
#define	BUTTON_PUM_PLANTS8		V92
#define	BUTTON_AUTO_PLANTS8		V93
#define	HIGHER_LIMIT_PLANTS8		V94
#define	LOWER_LIMIT_PLANTS8		V95
#define	CHART_HUMI_PLANTS8		V96
#define	CHART_SPRAY_PLANTS8		V93
#define	LED_STATUS_PLANTS8		V97



BlynkTimer						timer;
WidgetRTC						rtc;
WidgetTerminal					terminal(V0);
WidgetLED						LedPlants1(LED_STATUS_PLANTS1);
WidgetLED						LedPlants2(LED_STATUS_PLANTS2);
WidgetLED						LedPlants3(LED_STATUS_PLANTS3);
WidgetLED						LedPlants4(LED_STATUS_PLANTS4);
WidgetLED						LedPlants5(LED_STATUS_PLANTS5);
WidgetLED						LedPlants6(LED_STATUS_PLANTS6);
WidgetLED						LedPlants7(LED_STATUS_PLANTS7);
WidgetLED						LedPlants8(LED_STATUS_PLANTS8);

//Data Json
#define SPRAY					"S"
#define	S_HUMI					"H"
#define	ON						"T"
#define	OFF						"F"

//Define Harware configuration
#ifdef HieuTest
#define PUM_MAIN		D2
#define LOW_WARTER		D6
#define HIGH_WARTER		D7
#define LED_STT			D4
#else

#endif

//#define	In(pin)			pinMode(pin, INPUT)
//#define	Ou(pin)			pinMode(pin, OUTPUT)

//Value
bool	sttSendN = 0;
bool	levelWarter[2];
bool	SttPumMain;
bool	ActivityStt;

bool	SttHumi[8],	SttBAPlans[8];
bool	SttSpray[8], BufSttSpray[8];
int		limLow[8], limHigh[8];
int		SHumidity[8];

enum LevelWarter
{
	lowWater, HighWarter
};
enum Plants
{
	plants_1, plants_2, plants_3, plants_4, plants_5, plants_6, plants_7, plants_8
};


//Function
void ConfigIO() {
	pinMode(LOW_WARTER, INPUT);
	pinMode(HIGH_WARTER, INPUT);
	pinMode(PUM_MAIN, OUTPUT);
	digitalWrite(PUM_MAIN,LOW);
	pinMode(LED_STT, OUTPUT);
	digitalWrite(LED_STT, LOW);
}

void SendData(void) {
	DynamicJsonBuffer JsBufSend(256);
	JsonObject& JsSend = JsBufSend.createObject();
	for (uint8_t i = plants_1; i <= plants_8; i++) {
		JsSend[SPRAY + String(i)] = SttSpray[i] == HIGH ? ON : OFF;
	}
	String DataSend;
	JsSend.printTo(DataSend);
	if (Serial.available() >= 0) {
		Serial.println(DataSend);
	}
	return;
}
void updateNode(uint8_t time_s) {
	uint8_t i = plants_1;
	static unsigned long timeud = millis();
	
	if (millis() - timeud > (time_s * 1000)) {
			timeud = millis();
			sttSendN = HIGH;
		}
	while (i <= plants_8) {
		if (SttSpray[i] != BufSttSpray[i]) {
			timeud = millis();
			sttSendN = HIGH;
			i = plants_8;
		}
		i++;
	}
	if (sttSendN == HIGH) {
		sttSendN = LOW;
		SendData();
	}
}
void RecevierData(void) {
	if (Serial.available()) {
		static String Messenger = "";
		Messenger.reserve(256);
		Messenger = Serial.readString();

		DynamicJsonBuffer JsBufRecevie(256);
		JsonObject& JsRecevie = JsBufRecevie.parseObject(Messenger);
		if (!JsRecevie.success()) {
			return;
		}
		for (uint8_t i = plants_1; i <= plants_8; i++) {
			int HumiRece = JsRecevie[S_HUMI + String(i)];
			SHumidity[i] = map(HumiRece,0,1023,0,100);
			Blynk.virtualWrite(CHART_HUMI_PLANTS1 + 10 * i, SHumidity[i]);
			Blynk.virtualWrite(DISPLAY_PLANTS1 + 10 * i, SHumidity[i]);

			String SprayRece = JsRecevie[SPRAY + String(i)];
			SprayRece == ON ? BufSttSpray[i] = HIGH : BufSttSpray[i] = LOW;
			controlLedP(i, BufSttSpray[i]);
		}
	}
}
void checkLevelHumi(void) {
	for (uint8_t i = plants_1; i <= plants_8; i++) {
		if (limLow[i] > SHumidity[i]) {
			SttHumi[i] = false;
		}
		else if(limHigh[i] < SHumidity[i]) {
			SttHumi[i] = true;
		}
	}
}
void ControlPumNode(void) {
	for (uint8_t i = plants_1; i <= plants_8; i++) {
		if (SttBAPlans[i] == HIGH) {
			if (SttHumi[i] == true) {
				if(SttSpray[i] != LOW){
					SttSpray[i] = LOW;
					Blynk.virtualWrite(BUTTON_PUM_PLANTS1 + 10 * i, SttSpray[i]);
				}
			}
			else {
				if (SttSpray[i] != HIGH) {
					SttSpray[i] = HIGH;
					Blynk.virtualWrite(BUTTON_PUM_PLANTS1 + 10 * i, SttSpray[i]);
				}
			}
		}
		else
		{
			if (SHumidity[i] >= 99) {
				if (SttSpray[i] != LOW) {
					SttSpray[i] = LOW;
					Blynk.virtualWrite(BUTTON_PUM_PLANTS1 + 10 * i, SttSpray[i]);
				}
			}
		}
	}
}
int  CheckLeveWater(void) {
	levelWarter[lowWater] = digitalRead(LOW_WARTER);
	levelWarter[HighWarter] = digitalRead(HIGH_WARTER);

	if ((levelWarter[lowWater] == HIGH) && (levelWarter[HighWarter] == HIGH)) {
		Blynk.virtualWrite(WATER_LEVEL, 2);
		Blynk.virtualWrite(DISPLAY_MAIN, "High level warter");
		return 2;
	}
	else if ((levelWarter[lowWater] == HIGH) && (levelWarter[HighWarter] == LOW)){
		Blynk.virtualWrite(WATER_LEVEL, 1);
		Blynk.virtualWrite(DISPLAY_MAIN, "Mentium");
		return 1;
	}
	else if ((levelWarter[lowWater] == LOW) && (levelWarter[HighWarter] == LOW)) {
		Blynk.virtualWrite(WATER_LEVEL, 0);
		Blynk.virtualWrite(DISPLAY_MAIN, "Low level warter!");
		return 0;
	}
	else if ((levelWarter[lowWater] == LOW) && (levelWarter[HighWarter] == HIGH)) {
		Blynk.virtualWrite(WATER_LEVEL, -1);
		Blynk.virtualWrite(DISPLAY_MAIN, "Error senser warter!");
		return -1;
	}
}
void ControlPumMain(void) {

	int sttLevelW = CheckLeveWater();

	if (ActivityStt == HIGH) {
		switch (sttLevelW)
		{
		case 0:
			if (SttPumMain != HIGH) {
				digitalWrite(PUM_MAIN, HIGH);
				SttPumMain = HIGH;
				Blynk.virtualWrite(BUTTON_PUM_MAIN, HIGH);
			}
			break;
		case 1:
			digitalWrite(PUM_MAIN, SttPumMain);
			break;
		case 2:
			if (SttPumMain != LOW) {
				digitalWrite(PUM_MAIN, LOW);
				SttPumMain = LOW;
				Blynk.virtualWrite(BUTTON_PUM_MAIN, LOW);
			}
		default:

			break;
		}
	}
	else {
		digitalWrite(PUM_MAIN, SttPumMain);
	}
}
void controlLedP(uint8_t led, bool stt) {
	switch (led)
	{
	case plants_1:
		if (stt == HIGH) {
			LedPlants1.on();
		}
		else 
			LedPlants1.off();
		break;
	case plants_2:
		if (stt == HIGH) {
			LedPlants2.on();
		}
		else 
			LedPlants2.off();
		break;
	case plants_3:
		if (stt == HIGH) {
			LedPlants3.on();
		}
		else 
			LedPlants3.off();
		break;
	case plants_4:
		if (stt == HIGH) {
			LedPlants4.on();
		}
		else 
			LedPlants4.off();
		break;
	case plants_5:
		if (stt == HIGH) {
			LedPlants5.on();
		}
		else 
			LedPlants5.off();
		break;
	case plants_6:
		if (stt == HIGH) {
			LedPlants6.on();
		}
		else
			LedPlants6.off();
		break;
	case plants_7:
		if (stt == HIGH) {
			LedPlants7.on();
		}
		else 
			LedPlants7.off();
		break;
	case plants_8:
		if (stt == HIGH) {
			LedPlants8.on();
		}
		else 
			LedPlants8.off();
		break;
	}
}

BLYNK_CONNECTED() {
	rtc.begin();
	Blynk.syncVirtual(BUTTON_PUM_MAIN);
	Blynk.syncVirtual(BUTTON_AUTO);

	Blynk.syncVirtual(BUTTON_PUM_PLANTS1);
	Blynk.syncVirtual(BUTTON_AUTO_PLANTS1);
	Blynk.syncVirtual(HIGHER_LIMIT_PLANTS1);
	Blynk.syncVirtual(LOWER_LIMIT_PLANTS1);

	Blynk.syncVirtual(BUTTON_PUM_PLANTS2);
	Blynk.syncVirtual(BUTTON_AUTO_PLANTS2);
	Blynk.syncVirtual(HIGHER_LIMIT_PLANTS2);
	Blynk.syncVirtual(LOWER_LIMIT_PLANTS2);

	Blynk.syncVirtual(BUTTON_PUM_PLANTS3);
	Blynk.syncVirtual(BUTTON_AUTO_PLANTS3);
	Blynk.syncVirtual(HIGHER_LIMIT_PLANTS3);
	Blynk.syncVirtual(LOWER_LIMIT_PLANTS3);

	Blynk.syncVirtual(BUTTON_PUM_PLANTS4);
	Blynk.syncVirtual(BUTTON_AUTO_PLANTS4);
	Blynk.syncVirtual(HIGHER_LIMIT_PLANTS4);
	Blynk.syncVirtual(LOWER_LIMIT_PLANTS4);

	Blynk.syncVirtual(BUTTON_PUM_PLANTS5);
	Blynk.syncVirtual(BUTTON_AUTO_PLANTS5);
	Blynk.syncVirtual(HIGHER_LIMIT_PLANTS5);
	Blynk.syncVirtual(LOWER_LIMIT_PLANTS5);

	Blynk.syncVirtual(BUTTON_PUM_PLANTS6);
	Blynk.syncVirtual(BUTTON_AUTO_PLANTS6);
	Blynk.syncVirtual(HIGHER_LIMIT_PLANTS6);
	Blynk.syncVirtual(LOWER_LIMIT_PLANTS6);

	Blynk.syncVirtual(BUTTON_PUM_PLANTS7);
	Blynk.syncVirtual(BUTTON_AUTO_PLANTS7);
	Blynk.syncVirtual(HIGHER_LIMIT_PLANTS7);
	Blynk.syncVirtual(LOWER_LIMIT_PLANTS7);

	Blynk.syncVirtual(BUTTON_PUM_PLANTS8);
	Blynk.syncVirtual(BUTTON_AUTO_PLANTS8);
	Blynk.syncVirtual(HIGHER_LIMIT_PLANTS8);
	Blynk.syncVirtual(LOWER_LIMIT_PLANTS8);

	CheckLeveWater();
	SendData();
}
//ControlMain
BLYNK_WRITE(BUTTON_PUM_MAIN) {
	SttPumMain = param.asInt();
}
BLYNK_WRITE(BUTTON_AUTO) {
	ActivityStt = param.asInt();
}

//Plants1
BLYNK_WRITE(BUTTON_PUM_PLANTS1) {
	SttSpray[plants_1] = param.asInt();
}
BLYNK_WRITE(BUTTON_AUTO_PLANTS1) {
	SttBAPlans[plants_1] = param.asInt();
}
BLYNK_WRITE(HIGHER_LIMIT_PLANTS1) {
	limHigh[plants_1] = param.asInt();
	if (limHigh[plants_1] < limLow[plants_1]) {
		limHigh[plants_1] = limLow[plants_1];
		Blynk.virtualWrite(HIGHER_LIMIT_PLANTS1, limHigh[plants_1]);
	}
}
BLYNK_WRITE(LOWER_LIMIT_PLANTS1) {
	limLow[plants_1] = param.asInt();
	if (limHigh[plants_1] < limLow[plants_1]) {
		limLow[plants_1] = limHigh[plants_1];
		Blynk.virtualWrite(LOWER_LIMIT_PLANTS1, limLow[plants_1]);
	}
}

//Plants2
BLYNK_WRITE(BUTTON_PUM_PLANTS2) {
	SttSpray[plants_2] = param.asInt();
}
BLYNK_WRITE(BUTTON_AUTO_PLANTS2) {
	SttBAPlans[plants_2] = param.asInt();
}
BLYNK_WRITE(HIGHER_LIMIT_PLANTS2) {
	limHigh[plants_2] = param.asInt();
	if (limHigh[plants_2] < limLow[plants_2]) {
		limHigh[plants_2] = limLow[plants_2];
		Blynk.virtualWrite(HIGHER_LIMIT_PLANTS2, limHigh[plants_2]);
	}
}
BLYNK_WRITE(LOWER_LIMIT_PLANTS2) {
	limLow[plants_2] = param.asInt();
	if (limHigh[plants_2] < limLow[plants_2]) {
		limLow[plants_2] = limHigh[plants_2];
		Blynk.virtualWrite(LOWER_LIMIT_PLANTS2, limLow[plants_2]);
	}
}

//Plant3
BLYNK_WRITE(BUTTON_PUM_PLANTS3) {
	SttSpray[plants_3] = param.asInt();
}
BLYNK_WRITE(BUTTON_AUTO_PLANTS3) {
	SttBAPlans[plants_3] = param.asInt();
}
BLYNK_WRITE(HIGHER_LIMIT_PLANTS3) {
	limHigh[plants_3] = param.asInt();
	if (limHigh[plants_3] < limLow[plants_3]) {
		limHigh[plants_3] = limLow[plants_3];
		Blynk.virtualWrite(HIGHER_LIMIT_PLANTS3, limHigh[plants_3]);
	}
}
BLYNK_WRITE(LOWER_LIMIT_PLANTS3) {
	limLow[plants_3] = param.asInt();
	if (limHigh[plants_3] < limLow[plants_3]) {
		limLow[plants_3] = limHigh[plants_3];
		Blynk.virtualWrite(LOWER_LIMIT_PLANTS3, limLow[plants_3]);
	}
}

//Plants4
BLYNK_WRITE(BUTTON_PUM_PLANTS4) {
	SttSpray[plants_4] = param.asInt();
}
BLYNK_WRITE(BUTTON_AUTO_PLANTS4) {
	SttBAPlans[plants_4] = param.asInt();
}
BLYNK_WRITE(HIGHER_LIMIT_PLANTS4) {
	limHigh[plants_4] = param.asInt();
	if (limHigh[plants_4] < limLow[plants_4]) {
		limHigh[plants_4] = limLow[plants_4];
		Blynk.virtualWrite(HIGHER_LIMIT_PLANTS4, limHigh[plants_4]);
	}
}
BLYNK_WRITE(LOWER_LIMIT_PLANTS4) {
	limLow[plants_4] = param.asInt();
	if (limHigh[plants_4] < limLow[plants_4]) {
		limLow[plants_4] = limHigh[plants_4];
		Blynk.virtualWrite(LOWER_LIMIT_PLANTS4, limLow[plants_4]);
	}
}

//Plants5
BLYNK_WRITE(BUTTON_PUM_PLANTS5) {
	SttSpray[plants_5] = param.asInt();
}
BLYNK_WRITE(BUTTON_AUTO_PLANTS5) {
	SttBAPlans[plants_5] = param.asInt();
}
BLYNK_WRITE(HIGHER_LIMIT_PLANTS5) {
	limHigh[plants_5] = param.asInt();
	if (limHigh[plants_5] < limLow[plants_5]) {
		limHigh[plants_5] = limLow[plants_5];
		Blynk.virtualWrite(HIGHER_LIMIT_PLANTS5, limHigh[plants_5]);
	}
}
BLYNK_WRITE(LOWER_LIMIT_PLANTS5) {
	limLow[plants_5] = param.asInt();
	if (limHigh[plants_5] < limLow[plants_5]) {
		limLow[plants_5] = limHigh[plants_5];
		Blynk.virtualWrite(LOWER_LIMIT_PLANTS5, limLow[plants_5]);
	}
}

//Plants6
BLYNK_WRITE(BUTTON_PUM_PLANTS6) {
	SttSpray[plants_6] = param.asInt();
}
BLYNK_WRITE(BUTTON_AUTO_PLANTS6) {
	SttBAPlans[plants_6] = param.asInt();
}
BLYNK_WRITE(HIGHER_LIMIT_PLANTS6) {
	limHigh[plants_6] = param.asInt();
	if (limHigh[plants_6] < limLow[plants_6]) {
		limHigh[plants_6] = limLow[plants_6];
		Blynk.virtualWrite(HIGHER_LIMIT_PLANTS6, limHigh[plants_6]);
	}
}
BLYNK_WRITE(LOWER_LIMIT_PLANTS6) {
	limLow[plants_6] = param.asInt();
	if (limHigh[plants_6] < limLow[plants_6]) {
		limLow[plants_6] = limHigh[plants_6];
		Blynk.virtualWrite(LOWER_LIMIT_PLANTS6, limLow[plants_6]);
	}
}

//Plants7
BLYNK_WRITE(BUTTON_PUM_PLANTS7) {
	SttSpray[plants_7] = param.asInt();
}
BLYNK_WRITE(BUTTON_AUTO_PLANTS7) {
	SttBAPlans[plants_7] = param.asInt();
}
BLYNK_WRITE(HIGHER_LIMIT_PLANTS7) {
	limHigh[plants_7] = param.asInt();
	if (limHigh[plants_7] < limLow[plants_7]) {
		limHigh[plants_7] = limLow[plants_7];
		Blynk.virtualWrite(HIGHER_LIMIT_PLANTS7, limHigh[plants_7]);
	}
}
BLYNK_WRITE(LOWER_LIMIT_PLANTS7) {
	limLow[plants_7] = param.asInt();
	if (limHigh[plants_7] < limLow[plants_7]) {
		limLow[plants_7] = limHigh[plants_7];
		Blynk.virtualWrite(LOWER_LIMIT_PLANTS7, limLow[plants_7]);
	}
}

//Plants8
BLYNK_WRITE(BUTTON_PUM_PLANTS8) {
	SttSpray[plants_8] = param.asInt();
}
BLYNK_WRITE(BUTTON_AUTO_PLANTS8) {
	SttBAPlans[plants_8] = param.asInt();
}
BLYNK_WRITE(HIGHER_LIMIT_PLANTS8) {
	limHigh[plants_8] = param.asInt();
	if (limHigh[plants_8] < limLow[plants_8]) {
		limHigh[plants_8] = limLow[plants_8];
		Blynk.virtualWrite(HIGHER_LIMIT_PLANTS8, limHigh[plants_8]);
	}
}
BLYNK_WRITE(LOWER_LIMIT_PLANTS8) {
	limLow[plants_8] = param.asInt();
	if (limHigh[plants_8] < limLow[plants_8]) {
		limLow[plants_8] = limHigh[plants_8];
		Blynk.virtualWrite(LOWER_LIMIT_PLANTS8, limLow[plants_8]);
	}
}



//Information
String Owner = "tdhieu756@gmail.com";
String Version = "V1.8.9";
void setup()
{
	Serial.begin(115200);
	ConfigIO();
	delay(500);

	WiFiManager wifiManager;
	wifiManager.autoConnect("AutoConnectAP");
	Serial.print("Connect wifi: ");
	Serial.println(WiFi.SSID());
	Blynk.config(auth, "gith.cf", 8442);
}

// Add the main program code into the continuous loop() function
void loop()
{
	Blynk.run();
	RecevierData();
	checkLevelHumi();
	updateNode(10);
	ControlPumNode();
	ControlPumMain();
}
