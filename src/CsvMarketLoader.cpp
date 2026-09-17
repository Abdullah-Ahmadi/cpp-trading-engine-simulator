#include "exchange/CsvMarketLoader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace exchange {
namespace {

std::vector<std::string> splitCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream stream(line);
    std::string field;
    while (std::getline(stream, field, ',')) fields.push_back(field);
    return fields;
}

} // namespace

std::vector<Order> CsvMarketLoader::load(const std::string& path) {
    std::ifstream input(path);
    if (!input) throw std::runtime_error("Unable to open market data file: " + path);

    std::vector<Order> orders;
    std::string line;
    std::size_t lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        if (line.empty()) continue;
        if (lineNumber == 1 && line.rfind("timestamp,", 0) == 0) continue;

        const auto fields = splitCsvLine(line);
        if (fields.size() != 5) {
            throw std::runtime_error("Invalid CSV column count on line " + std::to_string(lineNumber));
        }

        try {
            Order order;
            order.timestamp = fields[0];
            order.symbol = fields[1];
            order.side = sideFromString(fields[2]);
            order.price = std::stod(fields[3]);
            order.quantity = std::stod(fields[4]);
            order.owner = "market";
            if (order.price <= 0.0 || order.quantity <= 0.0) {
                throw std::invalid_argument("price and quantity must be positive");
            }
            orders.push_back(order);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid market data on line " + std::to_string(lineNumber) + ": " + e.what());
        }
    }

    if (orders.empty()) throw std::runtime_error("Market data file contains no orders");
    return orders;
}

} // namespace exchange
