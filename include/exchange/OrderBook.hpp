#pragma once

#include "exchange/Order.hpp"
#include <optional>
#include <string>
#include <vector>

namespace exchange {

struct MarketStats {
    std::size_t bidCount{};
    std::size_t askCount{};
    std::optional<double> highestBid;
    std::optional<double> lowestAsk;
};

class OrderBook {
public:
    explicit OrderBook(std::vector<Order> orders = {});

    std::uint64_t addOrder(Order order);
    std::vector<std::string> symbols() const;
    std::vector<std::string> timestamps() const;
    MarketStats stats(const std::string& symbol, const std::string& timestamp) const;
    std::vector<Trade> match(const std::string& symbol, const std::string& timestamp);

private:
    std::vector<Order> orders_;
    std::uint64_t nextId_{1};
};

} // namespace exchange
