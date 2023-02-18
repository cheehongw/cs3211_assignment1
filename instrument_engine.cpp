#include "instrument_engine.hpp"

is_fulfilled InstrumentEngine::match_order(Order& order) {
    if (order.is_sell_order) {
        match_resting_buy(order);
    } else {
        match_resting_sell(order);
    }
}

is_fulfilled InstrumentEngine::match_resting_buy(Order& sell) 
{   

    while (sell.count > 0) {
        Order& best_resting_order = resting_buys.top();

        if (best_resting_order.is_canceled) {
            resting_buys.pop();
            continue;
        }

        if (best_resting_order.price > sell.price) {
            update_result match_result = best_resting_order.update_order(sell.count);
            sell.count -= match_result.units_traded; 

            if (match_result.is_resting_order_fully_filled) {
                resting_buys.pop();
            }


            intmax_t output_time = getCurrentTimestamp();
            Output::OrderExecuted(
                best_resting_order.order_id, 
                sell.order_id, 
                match_result.execution_id,
                best_resting_order.price,
                match_result.units_traded,
                output_time);

        } else {
            add_to_resting_sell(sell);
            return false; //no matching orders
        }
    }

    return true;
}

is_fulfilled InstrumentEngine::match_resting_sell(Order& buy) 
{   

    while (buy.count > 0) {
        Order& best_resting_order = resting_sells.top();

        if (best_resting_order.is_canceled) {
            resting_sells.pop();
            continue;
        }

        if (best_resting_order.price < buy.price) {
            update_result match_result = best_resting_order.update_order(buy.count);
            buy.count -= match_result.units_traded; 

            if (match_result.is_resting_order_fully_filled) {
                resting_sells.pop();
            }

            intmax_t output_time = getCurrentTimestamp();

            Output::OrderExecuted(
                best_resting_order.order_id, 
                buy.order_id, 
                match_result.execution_id,
                best_resting_order.price,
                match_result.units_traded,
                output_time);
        
        } else {
            add_to_resting_buy(buy);
            return false; //no matching orders
        }
    }

    return true;
}

void InstrumentEngine::add_to_resting_buy(Order& buy) 
{
    resting_buys.push(buy);
    intmax_t output_time = getCurrentTimestamp();
    Output::OrderAdded(buy.order_id, 
        buy.instrument.c_str(), buy.price, buy.count, buy.is_sell_order, output_time);
}


void InstrumentEngine::add_to_resting_sell(Order& sell) 
{
    resting_sells.push(sell);
    intmax_t output_time = getCurrentTimestamp();
    Output::OrderAdded(sell.order_id, 
        sell.instrument.c_str(), sell.price, sell.count, sell.is_sell_order, output_time);
}


bool InstrumentEngine::cancel_order(Order& order) 
{
    bool is_successful = order.cancel_order();
    return is_successful;
}