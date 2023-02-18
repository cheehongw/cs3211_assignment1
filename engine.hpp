// This file contains declarations for the main Engine class. You will
// need to add declarations to this file as you develop your Engine.

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <chrono>
#include <map>
#include <queue>
#include <string>

#include "io.hpp"
#include "instrument_engine.hpp"

struct Engine
{
public:
	void accept(ClientConnection conn);

private:
	std::unordered_map<std::string, InstrumentEngine&> Engine::instrument_engines;
	std::unordered_map<int, Order&> orders;

	void connection_thread(ClientConnection conn);
};

inline std::chrono::microseconds::rep getCurrentTimestamp() noexcept
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

#endif
