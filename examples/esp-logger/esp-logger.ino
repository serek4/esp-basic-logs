#include <esp-basic-logs.h>
#include <esp-basic-time.h>
#include <esp-basic-web-server.h>
#include <esp-basic-wifi.h>

#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASS "your-wifi-password"

#define NTP_SERVER_ADDRESS "0.pool.ntp.org"
#define NTP_SERVER_PORT 123
#define TIMEZONE 1    // Central European Time (Europe/Warsaw)

BasicWiFi wifi(WIFI_SSID, WIFI_PASS);
BasicTime NTPclient(NTP_SERVER_ADDRESS, NTP_SERVER_PORT, TIMEZONE);
BasicWebServer webServer;
BasicLogs logger;

u_long loopDelay = 0;

void setup() {
	Serial.begin(115200);
	Serial.println();
	filesystem.setup(true);
	wifi.addLogger(&BasicLogs::saveLog);
	wifi.onGotIP(handleWiFiGotIP);
	wifi.setup();
	webServer.setup();
	NTPclient.setup();
	if (wifi.waitForConnection() == BasicWiFi::wifi_got_ip) {
	}
	Serial.println("setup done!");
}

void loop() {
	NTPclient.handle();
	if (millis() - loopDelay >= 60000) {
		logger.saveLog(BasicLogs::_info_, "loop", NTPclient.dateTimeString());
		loopDelay = millis();
	}
	logger.handle();
}

void handleWiFiGotIP(GOT_IP_HANDLER_ARGS) {
	webServer.begin();
}
