#ifndef INSTRUMENT_ENGINE_HPP
#define INSTRUMENT_ENGINE_HPP

#include <queue> //replace this with a concurrent pq
#include <vector>
#include <unordered_set>
#include <memory>
#include <mutex>

#include "Order.hpp"
#include "io.hpp"
#include "getTimeStamp.hpp"


using is_fulfilled = bool;

struct Compare
{
    bool operator()(const std::shared_ptr<Order>& lhs, const std::shared_ptr<Order>&  rhs)
    {
        if (lhs->is_sell_order)
        {
            return lhs->price == rhs->price
                       ? lhs->time_order_added < rhs->time_order_added
                       : lhs->price > rhs->price;
        }
        else
        {
            return lhs->price == rhs->price
                       ? lhs->time_order_added < rhs->time_order_added
                       : lhs->price < rhs->price;
        }
    }
};


/**
 * Includes the engine for matching between buy and sell orders
 * of the same instrument
 */

struct InstrumentEngine
{

private:
    std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, Compare> resting_buys;
    std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, Compare> resting_sells;
    std::unordered_set<uint32_t>& existing_orders;
public:
    InstrumentEngine(std::unordered_set<uint32_t>& existing_orders_): existing_orders(existing_orders_) {};

    /**
     * Performs match_resting_buy or match_resting sell depending on the order type.
     *
     * @return false if order is not filled and added as resting order
     */
    is_fulfilled match_order(std::shared_ptr<Order> order);

    is_fulfilled match_resting_buy(std::shared_ptr<Order> sell);
    is_fulfilled match_resting_sell(std::shared_ptr<Order> buy);
    void add_to_resting_sell(std::shared_ptr<Order> sell);
    void add_to_resting_buy(std::shared_ptr<Order> buy);
    bool cancel_order(std::shared_ptr<Order> order);
};

#endif