#include "exchange/OrderBook.hpp"
#include "exchange/Wallet.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

namespace {

bool closeEnough(double a, double b) {
    return std::fabs(a - b) < 1e-9;
}

void testOrderMatching() {
    std::vector<exchange::Order> orders{
        {0, "T1", "BTC/USDT", exchange::Side::Sell, 100.0, 2.0, "seller"},
        {0, "T1", "BTC/USDT", exchange::Side::Buy, 110.0, 1.0, "buyer"}
    };
    exchange::OrderBook book(std::move(orders));
    const auto trades = book.match("BTC/USDT", "T1");
    assert(trades.size() == 1);
    assert(closeEnough(trades[0].price, 100.0));
    assert(closeEnough(trades[0].quantity, 1.0));
    assert(trades[0].buyer == "buyer");
    assert(trades[0].seller == "seller");
}

void testWallet() {
    exchange::Wallet wallet;
    wallet.deposit("USDT", 1000.0);
    assert(wallet.canPlace("BTC/USDT", exchange::Side::Buy, 100.0, 2.0));

    exchange::Trade trade{"T1", "BTC/USDT", 100.0, 2.0, "me", "market"};
    wallet.applyTrade(trade, "me");
    assert(closeEnough(wallet.balance("BTC"), 2.0));
    assert(closeEnough(wallet.balance("USDT"), 800.0));
}

} // namespace

int main() {
    testOrderMatching();
    testWallet();
    std::cout << "All tests passed.\n";
    return 0;
}
