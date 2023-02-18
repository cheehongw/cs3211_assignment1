#ifndef INSTRUMENT_ENGINE_HPP
#define INSTRUMENT_ENGINE_HPP

#include <queue>   //replace this with a concurrent pq

#include "Order.hpp"
#include "io.hpp"
#include "engine.hpp"


using is_fulfilled = bool;
/**
 * Includes the engine for matching between buy and sell orders
 * of the same instrument
 */

struct InstrumentEngine 
{


    private:
        std::priority_queue<Order&> resting_buys;
        std::priority_queue<Order&> resting_sells;

    public:

        /**
         * Performs match_resting_buy or match_resting sell depending on the order type.
         * 
         * @return true if order is not filled and added as resting order
         */
        is_fulfilled match_order(Order& order);

        is_fulfilled match_resting_buy(Order& sell);
        is_fulfilled match_resting_sell(Order& buy);
        void add_to_resting_sell(Order& sell);
        void add_to_resting_buy(Order& buy);
        bool cancel_order(Order& order);

};


#endif