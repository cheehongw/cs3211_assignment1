#include "instrument_engine.hpp"
#include <mutex>


is_fulfilled InstrumentEngine::match_order(std::shared_ptr<Order> order) {
    if (order->is_sell_order) {
        return match_resting_buy(order);
    } else {
        return match_resting_sell(order);
    }
}

is_fulfilled InstrumentEngine::match_resting_buy(std::shared_ptr<Order> sell) 
{   

    while (sell->count > 0) {


        if (resting_buys.empty() ) {
            add_to_resting_sell(sell);
            return false;
        }

        const std::shared_ptr<Order>& best_resting_order = resting_buys.top();

        intmax_t output_time = getCurrentTimestamp();
        
        if (!existing_orders.contains(best_resting_order->order_id)) {
            resting_buys.pop();
            continue;
        }

        if (best_resting_order->price >= sell->price) {
            update_result match_result = best_resting_order->update_order(sell->count);
            sell->count -= match_result.units_traded; 

            if (match_result.is_resting_order_fully_filled) {
                resting_buys.pop();
                existing_orders.erase(best_resting_order->order_id);
            }

            Output::OrderExecuted(
                best_resting_order->order_id, 
                sell->order_id, 
                match_result.execution_id,
                best_resting_order->price,
                match_result.units_traded,
                output_time);

        } else {
            add_to_resting_sell(sell);
            return false; //no matching orders
        }
    }

    return true;
}

is_fulfilled InstrumentEngine::match_resting_sell(std::shared_ptr<Order> buy) 
{   
    
    while (buy->count > 0) {

        if (resting_sells.empty() ) {
            add_to_resting_buy(buy);
            return false;
        }

        const std::shared_ptr<Order>& best_resting_order = resting_sells.top();

        intmax_t output_time = getCurrentTimestamp();
        
        if (!existing_orders.contains(best_resting_order->order_id)) {
            resting_sells.pop();
            continue;
        }

        if (best_resting_order->price <= buy->price) {
            update_result match_result = best_resting_order->update_order(buy->count);
            buy->count -= match_result.units_traded; 

            if (match_result.is_resting_order_fully_filled) {
                resting_sells.pop();
                existing_orders.erase(best_resting_order->order_id);

            }

            Output::OrderExecuted(
                best_resting_order->order_id, 
                buy->order_id, 
                match_result.execution_id,
                best_resting_order->price,
                match_result.units_traded,
                output_time);
        
        } else {
            add_to_resting_buy(buy);
            return false; //no matching orders
        }
    }

    return true;
}

void InstrumentEngine::add_to_resting_buy(std::shared_ptr<Order> buy) 
{
    resting_buys.push(buy);
    intmax_t output_time = getCurrentTimestamp();
    Output::OrderAdded(buy->order_id, 
        buy->instrument.c_str(), buy->price, buy->count, buy->is_sell_order, output_time);
}


void InstrumentEngine::add_to_resting_sell(std::shared_ptr<Order> sell) 
{
    resting_sells.push(sell);
    intmax_t output_time = getCurrentTimestamp();
    Output::OrderAdded(sell->order_id, 
        sell->instrument.c_str(), sell->price, sell->count, sell->is_sell_order, output_time);
}


bool InstrumentEngine::cancel_order(std::shared_ptr<Order> order) 
{
    bool is_successful = order->cancel_order();
    return is_successful;
}