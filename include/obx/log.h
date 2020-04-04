#pragma once

#include <cstdint>
#include <string>
#include <iostream>

namespace obx {
namespace {

	enum class LogLevel : uint32_t {
		DEBUG = 0,
		INFO,
		ERROR,
		EXCEPTION
	};

	#ifndef GLOBAL_LOG_LEVEL
		#define GLOBAL_LOG_LEVEL DEBUG
	#endif

	inline constexpr LogLevel globalLogLevel = LogLevel::GLOBAL_LOG_LEVEL;

	template<LogLevel logLevel>
	static void log(const std::string& message) {
		if constexpr(logLevel < globalLogLevel) {
			return;
		}

		if constexpr(logLevel == LogLevel::DEBUG || logLevel == LogLevel::INFO) {
			std::cout << message << "\n";
		}
		else if constexpr(logLevel == LogLevel::ERROR) {
			std::cerr << message << std::endl;
		}
		else if constexpr(logLevel == LogLevel::EXCEPTION) {
			#if __cpp_exceptions
				throw std::runtime_error(message);
			#else
				std::cerr << message << std::endl;
				std::terminate();
			#endif
		}
	}

}
}
