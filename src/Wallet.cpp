#include "exchange/Wallet.hpp"
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace exchange {

std::pair<std::string, std::string> Wallet::splitSymbol(const std::string& symbol) {
    const auto slash = symbol.find('/');
    if (slash == std::string::npos || slash == 0 || slash + 1 >= symbol.size()) {
        throw std::invalid_argument("Expected symbol in BASE/QUOTE format: " + symbol);
    }
    return {symbol.substr(0, slash), symbol.substr(slash + 1)};
}

void Wallet::deposit(const std::string& currency, double amount) {
    if (amount < 0.0) throw std::invalid_argument("Deposit amount cannot be negative");
    balances_[currency] += amount;
}

double Wallet::balance(const std::string& currency) const {
    const auto it = balances_.find(currency);
    return it == balances_.end() ? 0.0 : it->second;
}

bool Wallet::canPlace(const std::string& symbol, Side side, double price, double quantity) const {
    if (price <= 0.0 || quantity <= 0.0) return false;
    const auto [base, quote] = splitSymbol(symbol);
    if (side == Side::Sell) return balance(base) + 1e-12 >= quantity;
    return balance(quote) + 1e-12 >= price * quantity;
}

void Wallet::applyTrade(const Trade& trade, const std::string& owner) {
    const auto [base, quote] = splitSymbol(trade.symbol);
    const double quoteAmount = trade.price * trade.quantity;

    if (trade.buyer == owner) {
        balances_[quote] -= quoteAmount;
        balances_[base] += trade.quantity;
    }
    if (trade.seller == owner) {
        balances_[base] -= trade.quantity;
        balances_[quote] += quoteAmount;
    }
}

std::string Wallet::summary() const {
    std::ostringstream out;
    out << std::fixed << std::setprecision(6);
    for (const auto& [currency, amount] : balances_) out << currency << ": " << amount << '\n';
    return out.str();
}

} // namespace exchange
