#include <iostream>
#include <thread>
#include <queue>

#include "io.hpp"
#include "engine.hpp"

void Engine::accept(ClientConnection connection)
{
	auto thread = std::thread(&Engine::connection_thread, this, std::move(connection));
	thread.detach();
}

void Engine::connection_thread(ClientConnection connection)
{
	while(true)
	{
		ClientCommand input {};
		switch(connection.readInput(input))
		{
			case ReadResult::Error: SyncCerr {} << "Error reading input" << std::endl;
			case ReadResult::EndOfFile: return;
			case ReadResult::Success: break;
		}

		// Functions for printing output actions in the prescribed format are
		// provided in the Output class:
		switch(input.type)
		{
			case input_cancel: {
				SyncCerr {} << "Got cancel: ID: " << input.order_id << std::endl;
				
				bool is_successful = false;
				auto to_delete = orders.find(input.order_id);
				if (to_delete != orders.end()) {
					is_successful = to_delete->second.cancel_order();
				}

				auto output_time = getCurrentTimestamp();
				Output::OrderDeleted(input.order_id, is_successful, output_time);
				break;
			}

			default: {
				SyncCerr {}
				    << "Got order: " << static_cast<char>(input.type) << " " << input.instrument << " x " << input.count << " @ "
				    << input.price << " ID: " << input.order_id << std::endl;

				//TODO:
				// initialize Order obj (not done)
				// fetch corresponding instrument engine (initialize if doesnt exist) (done)
				// match order in instrument engine (not done)
				// add order into engine list if added as resting order (not done)
				Order order = new Order();

				auto iter = instrument_engines.find(input.instrument);
				InstrumentEngine* instrument_engine;
				if (iter == instrument_engines.end()) {
					instrument_engine = new InstrumentEngine();
					instrument_engines.emplace(instrument_engine); //???? 
				} else {
					iter->second.match_order(order);
				}

				break;
			}
		}
	}
}
