#include "exchange/Order.hpp"
#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace exchange {

std::string toString(Side side) {
    return side == Side::Buy ? "buy" : "sell";
}

Side sideFromString(const std::string& value) {
    std::string normalized = value;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (normalized == "buy" || normalized == "bid") return Side::Buy;
    if (normalized == "sell" || normalized == "ask") return Side::Sell;
    throw std::invalid_argument("Unknown order side: " + value);
}

} // namespace exchange
