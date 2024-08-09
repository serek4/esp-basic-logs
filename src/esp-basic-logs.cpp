#include "esp-basic-logs.hpp"


String BasicLogs::_pendingLogs = "";
String BasicLogs::_logBuffer = "";
u_long BasicLogs::_saveLogDelayTimer = 0;
const char* BasicLogs::_logLevelStr[] = {"critical", "error", "warning", "info", "debug", "unknown"};

void BasicLogs::setup() {
	if (!(filesystem._fsStarted)) {
		BASIC_FS_PRINTLN("file system not mounted yet, mounting");
		filesystem.setup();
	}
}
void BasicLogs::saveLog(time_t time, uint8_t logLevel, String message) {
	logLevel = constrain(logLevel, _critical_, _unknown_);
	_pendingLogs += BasicTime::dateTimeString(time) + ", " + _logLevelStr[logLevel] + ", " + message + "\n";
}
void BasicLogs::saveLog(time_t time, String logLevel, String message) {
	_pendingLogs += BasicTime::dateTimeString(time) + ", " + logLevel + ", " + message + "\n";
}
void BasicLogs::handle() {
	if (_pendingLogs.length() > 0) {    // add pending logs to buffer and clear pending logs
		_logBuffer += _pendingLogs;
		_pendingLogs = "";
	}
	if (_logBuffer.length() > 0 && millis() - _saveLogDelayTimer >= SAVE_LOG_MIN_INTERVAL) {    // save logs from buffer and clear it on success
		File logFile;
		bool logSaved = false;
		if (!(filesystem.exists(LOG_FILE_NAME))) {
			String newFile = "time, logLevel, message\n" + _logBuffer;
			logSaved = filesystem.writeFile(newFile, LOG_FILE_NAME, logFile);
		} else {
			logSaved = filesystem.appendFile(_logBuffer, LOG_FILE_NAME, logFile);
		}
		if (!logSaved) {
			BASIC_LOGS_PRINTLN("log file update failed");
		} else {
			BASIC_LOGS_PRINTLN("saved logs\n====>");
			BASIC_LOGS_PRINT(_logBuffer);
			BASIC_LOGS_PRINTLN("<====");
			BASIC_LOGS_PRINTLN("pending logs\n====>");
			BASIC_LOGS_PRINT(_pendingLogs);
			BASIC_LOGS_PRINTLN("<====");
			_logBuffer = "";
		}
		_saveLogDelayTimer = millis();
	}
}
