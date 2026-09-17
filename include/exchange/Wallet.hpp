#pragma once

#include "exchange/Order.hpp"
#include <map>
#include <string>

namespace exchange {

class Wallet {
public:
    void deposit(const std::string& currency, double amount);
    double balance(const std::string& currency) const;
    bool canPlace(const std::string& symbol, Side side, double price, double quantity) const;
    void applyTrade(const Trade& trade, const std::string& owner);
    std::string summary() const;

private:
    std::map<std::string, double> balances_;
    static std::pair<std::string, std::string> splitSymbol(const std::string& symbol);
};

} // namespace exchange
