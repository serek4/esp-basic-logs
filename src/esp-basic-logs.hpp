#pragma once

#include <esp-basic-fs.hpp>
#include <esp-basic-time.hpp>

// #define BASIC_LOGS_DEBUG
// debug printing macros
// clang-format off
#ifdef BASIC_LOGS_DEBUG
#define DEBUG_PRINTER Serial
#define BASIC_LOGS_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define BASIC_LOGS_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#define BASIC_LOGS_PRINTF(...) { DEBUG_PRINTER.printf(__VA_ARGS__); }
#else
#define BASIC_LOGS_PRINT(...) {}
#define BASIC_LOGS_PRINTLN(...) {}
#define BASIC_LOGS_PRINTF(...) {}
#endif
// clang-format on

#define SAVE_LOG_MIN_INTERVAL 100
#define LOG_FILE_NAME "log.csv"

class BasicLogs {
  public:
	enum LogLevel {
		_none_,
		_critical_,
		_error_,
		_warning_,
		_info_,
		_debug_,
		_unknown_
	};
	void setup();
	static void handle();
	static void saveLog(time_t time, uint8_t logLevel, String origin, String message);
	static void saveLog(uint8_t logLevel, String origin, String message) { saveLog(now(), logLevel, origin, message); };
	static void saveLog(uint8_t logLevel, String message) { saveLog(now(), logLevel, emptyString, message); };
	static void saveLog(String origin, String message) { saveLog(now(), _none_, origin, message); };

  private:
	static String _pendingLogs;
	static String _logBuffer;
	static u_long _saveLogDelayTimer;
	static const char* _logLevelStr[];
};
