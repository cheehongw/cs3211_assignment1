#include "Order.hpp"

bool operator<(const Order &lhs, const Order &rhs)
{
    if (lhs.is_sell_order) {
        return lhs.price == rhs.price 
            ? lhs.time_order_added < rhs.time_order_added 
            : lhs.price > rhs.price;
    } else {
        return lhs.price == rhs.price 
            ? lhs.time_order_added < rhs.time_order_added 
            : lhs.price < rhs.price;
    }
}


update_result Order::update_order(uint32_t reduce_by)
{   
    update_result res;

    if (reduce_by >= count) {
        count = 0;
        res.units_traded = count;
        res.is_resting_order_fully_filled = true;

    } else {
        count = count - reduce_by;
        res.units_traded = reduce_by;
        res.is_resting_order_fully_filled = false;
    }

    res.execution_id = execution_id;
    execution_id++;

    return res;
}

bool Order::cancel_order()
{   
    if (is_canceled) {
        return false;
    } else {
        is_canceled = true;
        return true;
    }
}
