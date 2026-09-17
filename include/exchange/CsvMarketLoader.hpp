#pragma once

#include "exchange/Order.hpp"
#include <string>
#include <vector>

namespace exchange {

class CsvMarketLoader {
public:
    static std::vector<Order> load(const std::string& path);
};

} // namespace exchange
