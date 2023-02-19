// This file contains declarations for the main Engine class. You will
// need to add declarations to this file as you develop your Engine.

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <string>


#include "io.hpp"
#include "instrument_engine.hpp"
#include "Order.hpp"
#include "getTimeStamp.hpp"

struct Engine
{
public:
	void accept(ClientConnection conn);

private:
	std::unordered_map<std::string, InstrumentEngine> instrument_engines; 
	std::unordered_set<uint32_t> existing_orders;
	std::mutex mut;

	void connection_thread(ClientConnection conn);
};

#endif
