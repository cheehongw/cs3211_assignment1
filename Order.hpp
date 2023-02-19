#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>

struct update_result {
    uint32_t units_traded;
    uint32_t execution_id;
    bool is_resting_order_fully_filled;
};


class Order
{
public:
    bool is_sell_order;
    uint32_t order_id;
    uint32_t count;
    uint32_t price;
    std::string instrument;
    intmax_t time_order_added;
    uint32_t execution_id;

    bool is_canceled;

    Order(bool is_sell_order, uint32_t order_id, uint32_t price, uint32_t count, std::string instrument, intmax_t time_order_added) 
        : is_sell_order(is_sell_order), order_id(order_id), count(count), price(price),
        instrument(instrument), time_order_added(time_order_added), execution_id(1), is_canceled(false)
    {};

    //TODO: implement destructor
    ~Order() {};

    /**
     * Updates the order by reducing the count and incrementing
     * execution_id
     * 
     * Returns the number of units traded.
     */
    update_result update_order(uint32_t reduce_by);

    /**
     * cancels the order
     * 
     * @return true if the command to cancel was successful
     * 
     * @return false if the command to cancel was unsuccessful 
     */
    bool cancel_order();


    friend bool operator<(const Order& lhs, const Order& rhs);


};

#endif