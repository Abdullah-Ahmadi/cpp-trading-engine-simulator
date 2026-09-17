#include "exchange/CsvMarketLoader.hpp"
#include "exchange/OrderBook.hpp"
#include "exchange/Wallet.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

constexpr const char* kOwner = "portfolio-user";

void printHelp() {
    std::cout
        << "Commands:\n"
        << "  help                         Show this help\n"
        << "  products                     List available symbols\n"
        << "  stats <SYMBOL>               Show top-of-book stats for the current timestamp\n"
        << "  buy <SYMBOL> <PRICE> <QTY>   Place a limit buy order\n"
        << "  sell <SYMBOL> <PRICE> <QTY>  Place a limit sell order\n"
        << "  wallet                       Show simulated balances\n"
        << "  step                         Match orders and advance to the next timestamp\n"
        << "  trades                       Show trades involving the simulated user\n"
        << "  quit                         Exit\n";
}

std::vector<std::string> splitWords(const std::string& line) {
    std::stringstream input(line);
    std::vector<std::string> words;
    std::string word;
    while (input >> word) words.push_back(word);
    return words;
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        const std::string dataPath = argc > 1 ? argv[1] : "data/sample_orders.csv";
        exchange::OrderBook orderBook(exchange::CsvMarketLoader::load(dataPath));
        const auto timestamps = orderBook.timestamps();
        if (timestamps.empty()) throw std::runtime_error("No timestamps available");

        std::size_t timeIndex = 0;
        exchange::Wallet wallet;
        wallet.deposit("BTC", 1.0);
        wallet.deposit("ETH", 8.0);
        wallet.deposit("USDT", 15000.0);
        std::vector<exchange::Trade> userTrades;

        std::cout << "C++ Trading Engine Simulator\n"
                  << "Loaded " << timestamps.size() << " market snapshots from " << dataPath << "\n"
                  << "Type 'help' for commands.\n";

        for (std::string line; ; ) {
            const std::string& now = timestamps[timeIndex];
            std::cout << "\n[" << now << "] > ";
            if (!std::getline(std::cin, line)) break;
            const auto words = splitWords(line);
            if (words.empty()) continue;

            const std::string& command = words[0];
            if (command == "quit" || command == "exit") break;
            if (command == "help") {
                printHelp();
            } else if (command == "products") {
                for (const auto& symbol : orderBook.symbols()) std::cout << "  " << symbol << '\n';
            } else if (command == "wallet") {
                std::cout << wallet.summary();
            } else if (command == "stats") {
                if (words.size() != 2) {
                    std::cout << "Usage: stats <SYMBOL>\n";
                    continue;
                }
                const auto stats = orderBook.stats(words[1], now);
                std::cout << "Bids: " << stats.bidCount << ", asks: " << stats.askCount << '\n';
                std::cout << "Highest bid: " << (stats.highestBid ? std::to_string(*stats.highestBid) : "n/a") << '\n';
                std::cout << "Lowest ask:  " << (stats.lowestAsk ? std::to_string(*stats.lowestAsk) : "n/a") << '\n';
            } else if (command == "buy" || command == "sell") {
                if (words.size() != 4) {
                    std::cout << "Usage: " << command << " <SYMBOL> <PRICE> <QTY>\n";
                    continue;
                }
                try {
                    const double price = std::stod(words[2]);
                    const double quantity = std::stod(words[3]);
                    const auto side = command == "buy" ? exchange::Side::Buy : exchange::Side::Sell;
                    if (!wallet.canPlace(words[1], side, price, quantity)) {
                        std::cout << "Insufficient simulated balance or invalid order values.\n";
                        continue;
                    }
                    orderBook.addOrder(exchange::Order{0, now, words[1], side, price, quantity, kOwner});
                    std::cout << "Order accepted. It will be considered when this snapshot is matched.\n";
                } catch (const std::exception& e) {
                    std::cout << "Invalid order: " << e.what() << '\n';
                }
            } else if (command == "step") {
                std::size_t tradeCount = 0;
                for (const auto& symbol : orderBook.symbols()) {
                    auto trades = orderBook.match(symbol, now);
                    tradeCount += trades.size();
                    for (const auto& trade : trades) {
                        if (trade.buyer == kOwner || trade.seller == kOwner) {
                            wallet.applyTrade(trade, kOwner);
                            userTrades.push_back(trade);
                        }
                    }
                }
                std::cout << "Matched " << tradeCount << " trade(s).\n";
                timeIndex = (timeIndex + 1) % timestamps.size();
                std::cout << "Advanced to " << timestamps[timeIndex] << ".\n";
            } else if (command == "trades") {
                if (userTrades.empty()) {
                    std::cout << "No simulated-user trades yet.\n";
                    continue;
                }
                std::cout << std::fixed << std::setprecision(6);
                for (const auto& trade : userTrades) {
                    std::cout << trade.timestamp << " | " << trade.symbol << " | "
                              << trade.quantity << " @ " << trade.price
                              << " | buyer=" << trade.buyer << " seller=" << trade.seller << '\n';
                }
            } else {
                std::cout << "Unknown command. Type 'help'.\n";
            }
        }

        std::cout << "Goodbye.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }
}
