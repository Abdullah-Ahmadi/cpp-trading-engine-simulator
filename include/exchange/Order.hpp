#pragma once

#include <cstdint>
#include <string>

namespace exchange {

enum class Side { Buy, Sell };

struct Order {
    std::uint64_t id{};
    std::string timestamp;
    std::string symbol;
    Side side{Side::Buy};
    double price{};
    double quantity{};
    std::string owner{"market"};
};

struct Trade {
    std::string timestamp;
    std::string symbol;
    double price{};
    double quantity{};
    std::string buyer;
    std::string seller;
};

std::string toString(Side side);
Side sideFromString(const std::string& value);

} // namespace exchange
