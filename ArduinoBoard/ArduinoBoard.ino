#include <SPI.h>
#include <Ethernet.h>
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <BlynkSimpleEthernet.h>

#define LED_STT	LED_BUILTIN
#define RELAY1	2
#define RELAY2	3
#define RELAY3	4
#define RELAY4	5
#define RELAY5	6
#define RELAY6	7
#define RELAY7	8
#define RELAY8	9
#define RELAY9	11
#define RELAY10	12
#define RELAY11	A5
#define RELAY12	A4
#define RELAY13	A3
#define RELAY14	A2
#define RELAY15	A1
#define RELAY16	A0

BLYNK_WRITE(V1) {
	digitalWrite(RELAY1, param.asInt());
}
BLYNK_WRITE(V2) {
	digitalWrite(RELAY2, param.asInt());
}
BLYNK_WRITE(V3) {
	digitalWrite(RELAY3, param.asInt());
}
BLYNK_WRITE(V4) {
	digitalWrite(RELAY4, param.asInt());
}
BLYNK_WRITE(V5) {
	digitalWrite(RELAY5, param.asInt());
}
BLYNK_WRITE(V6) {
	digitalWrite(RELAY6, param.asInt());
}
BLYNK_WRITE(V7) {
	digitalWrite(RELAY7, param.asInt());
}
BLYNK_WRITE(V8) {
	digitalWrite(RELAY8, param.asInt());
}
BLYNK_WRITE(V9) {
	digitalWrite(RELAY9, param.asInt());
}
BLYNK_WRITE(V10) {
	digitalWrite(RELAY10, param.asInt());
}
BLYNK_WRITE(V11) {
	digitalWrite(RELAY11, param.asInt());
}
BLYNK_WRITE(V12) {
	digitalWrite(RELAY12, param.asInt());
}
BLYNK_WRITE(V13) {
	digitalWrite(RELAY13, param.asInt());
}
BLYNK_WRITE(V14) {
	digitalWrite(RELAY14, param.asInt());
}
BLYNK_WRITE(V15) {
	digitalWrite(RELAY15, param.asInt());
}
BLYNK_WRITE(V16) {
	digitalWrite(RELAY16, param.asInt());
}

BLYNK_CONNECTED() {
	digitalWrite(LED_STT, HIGH);
	Blynk.syncVirtual(V1);
	Blynk.syncVirtual(V2);
	Blynk.syncVirtual(V3);
	Blynk.syncVirtual(V4);
	Blynk.syncVirtual(V5);
	Blynk.syncVirtual(V6);
	Blynk.syncVirtual(V7);
	Blynk.syncVirtual(V8);
	Blynk.syncVirtual(V9);
	Blynk.syncVirtual(V10);
	Blynk.syncVirtual(V11);
	Blynk.syncVirtual(V12);
	Blynk.syncVirtual(V13);
	Blynk.syncVirtual(V14);
	Blynk.syncVirtual(V15);
	Blynk.syncVirtual(V16);
}

int pinIO[] = { RELAY1,RELAY2,RELAY3,RELAY4,RELAY5,RELAY6,RELAY7,RELAY8,RELAY9,RELAY10,RELAY11,RELAY12,RELAY13,RELAY14,RELAY15,RELAY16,LED_STT };
char auth[] = "f3c077970eae4dab88c0a55b3bd89ad8";
char host[] = "gith.cf";
int port = 8442;


void setup()
{
	Serial.begin(9600);
	configIO(); 
	Blynk.begin(auth, host, port);
	Serial.println("end config!");
}

void loop()
{
	Blynk.run();
}

void configIO(){
	Serial.println("setup IO");
	for (uint8_t i = 0; i <= 16; i++) {
		pinMode(pinIO[i], OUTPUT);
	}
	for (uint8_t i = 0; i <= 16; i++) {
		digitalWrite(pinIO[i], HIGH);
	}
}