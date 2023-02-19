#include "Order.hpp"

update_result Order::update_order(uint32_t reduce_by)
{   
    update_result res;

    if (reduce_by >= count) {
        res.units_traded = count;
        count = 0;
        res.is_resting_order_fully_filled = true;

    } else {
        res.units_traded = reduce_by;
        count = count - reduce_by;
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

