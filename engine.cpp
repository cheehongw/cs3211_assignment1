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
	std::lock_guard<std::mutex> lock{mut};
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
				auto to_delete = existing_orders.find(input.order_id);
				if (to_delete != existing_orders.end()) {
					existing_orders.erase(to_delete);
					is_successful = true;
				}

				auto output_time = getCurrentTimestamp();
				Output::OrderDeleted(input.order_id, is_successful, output_time);
				break;
			}

			default: {
				SyncCerr {}
				    << "Got order: " << static_cast<char>(input.type) << " " << input.instrument << " x " << input.count << " @ "
				    << input.price << " ID: " << input.order_id << std::endl;

			
				// initialize Order obj 
				// fetch corresponding instrument engine (initialize if doesnt exist) 
				// match order in instrument engine 
				// add order into engine list if added as resting order 

				auto order = std::make_shared<Order>(input.type == input_sell, input.order_id, 
					input.price, input.count, input.instrument, 0);
				
				auto result = instrument_engines.emplace(
					std::piecewise_construct,
						std::forward_as_tuple(input.instrument), 
						std::forward_as_tuple(existing_orders));

				InstrumentEngine& instr_eng = result.first->second;

				bool is_fulfilled = instr_eng.match_order(order);
				
				if (!is_fulfilled) {
					existing_orders.insert(order->order_id);
				}
				
				break;
			}
		}
	}
}
