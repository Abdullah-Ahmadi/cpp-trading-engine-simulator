#include "exchange/OrderBook.hpp"
#include <algorithm>
#include <set>
#include <stdexcept>

namespace exchange {

OrderBook::OrderBook(std::vector<Order> orders) : orders_(std::move(orders)) {
    for (auto& order : orders_) {
        if (order.id == 0) order.id = nextId_++;
        else nextId_ = std::max(nextId_, order.id + 1);
    }
}

std::uint64_t OrderBook::addOrder(Order order) {
    if (order.price <= 0.0 || order.quantity <= 0.0) {
        throw std::invalid_argument("Order price and quantity must be positive");
    }
    order.id = nextId_++;
    orders_.push_back(std::move(order));
    return orders_.back().id;
}

std::vector<std::string> OrderBook::symbols() const {
    std::set<std::string> unique;
    for (const auto& order : orders_) unique.insert(order.symbol);
    return {unique.begin(), unique.end()};
}

std::vector<std::string> OrderBook::timestamps() const {
    std::set<std::string> unique;
    for (const auto& order : orders_) unique.insert(order.timestamp);
    return {unique.begin(), unique.end()};
}

MarketStats OrderBook::stats(const std::string& symbol, const std::string& timestamp) const {
    MarketStats result;
    for (const auto& order : orders_) {
        if (order.symbol != symbol || order.timestamp != timestamp || order.quantity <= 0.0) continue;
        if (order.side == Side::Buy) {
            ++result.bidCount;
            if (!result.highestBid || order.price > *result.highestBid) result.highestBid = order.price;
        } else {
            ++result.askCount;
            if (!result.lowestAsk || order.price < *result.lowestAsk) result.lowestAsk = order.price;
        }
    }
    return result;
}

std::vector<Trade> OrderBook::match(const std::string& symbol, const std::string& timestamp) {
    std::vector<Order*> bids;
    std::vector<Order*> asks;

    for (auto& order : orders_) {
        if (order.symbol != symbol || order.timestamp != timestamp || order.quantity <= 0.0) continue;
        (order.side == Side::Buy ? bids : asks).push_back(&order);
    }

    std::sort(bids.begin(), bids.end(), [](const Order* a, const Order* b) {
        if (a->price != b->price) return a->price > b->price;
        return a->id < b->id;
    });
    std::sort(asks.begin(), asks.end(), [](const Order* a, const Order* b) {
        if (a->price != b->price) return a->price < b->price;
        return a->id < b->id;
    });

    std::vector<Trade> trades;
    std::size_t bidIndex = 0;
    std::size_t askIndex = 0;
    constexpr double epsilon = 1e-12;

    while (bidIndex < bids.size() && askIndex < asks.size()) {
        Order& bid = *bids[bidIndex];
        Order& ask = *asks[askIndex];
        if (bid.price + epsilon < ask.price) break;

        const double quantity = std::min(bid.quantity, ask.quantity);
        trades.push_back(Trade{timestamp, symbol, ask.price, quantity, bid.owner, ask.owner});
        bid.quantity -= quantity;
        ask.quantity -= quantity;

        if (bid.quantity <= epsilon) {
            bid.quantity = 0.0;
            ++bidIndex;
        }
        if (ask.quantity <= epsilon) {
            ask.quantity = 0.0;
            ++askIndex;
        }
    }

    return trades;
}

} // namespace exchange
