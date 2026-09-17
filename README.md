# C++ Trading Engine Simulator

A command-line trading engine simulator written in modern C++. It loads synthetic market snapshots from CSV, maintains limit order books, accepts simulated user orders, matches compatible bids and asks, and updates a multi-currency wallet when trades execute.

> **Portfolio note:** This independent project was developed after completing the University of London / Goldsmiths Object Oriented Programming specialization on Coursera, applying and extending concepts learned during the program.

## Why this project

I built this project to demonstrate practical use of object-oriented C++ beyond small isolated exercises. The code separates market data loading, order-book behavior, trade matching, wallet accounting, and the command-line interface into focused components.

## Features

- CSV market-data loading with validation and useful error messages
- Buy and sell limit orders
- Price-time-style matching within each market snapshot
- Multiple trading pairs using `BASE/QUOTE` symbols
- Simulated wallet with balance checks before placing orders
- Trade history for the simulated user
- Market statistics such as highest bid and lowest ask
- Reproducible synthetic sample data
- CMake build configuration
- Lightweight automated smoke tests with CTest

## Concepts demonstrated

- Classes and encapsulation
- Header/source separation
- STL containers and algorithms
- Enumerations and structured domain models
- File I/O and parsing
- Exceptions and input validation
- Sorting and matching algorithms
- Const-correct APIs
- Basic automated testing
- CMake-based project organization

## Project structure

```text
.
├── app/                    # CLI application
├── data/                   # Synthetic demo data
├── include/exchange/       # Public headers
├── src/                    # Core implementation
├── tests/                  # Smoke tests
├── CMakeLists.txt
└── README.md
```

## Build and run

### Requirements

- C++17-compatible compiler (GCC, Clang, or MSVC)
- CMake 3.16+

### Linux / macOS

```bash
cmake -S . -B build
cmake --build build
./build/trading_engine_sim
```

### Windows (PowerShell)

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\trading_engine_sim.exe
```

Depending on the CMake generator, the executable may also be created directly under `build/`.

## Run tests

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Example session

```text
C++ Trading Engine Simulator
Loaded 3 market snapshots from data/sample_orders.csv
Type 'help' for commands.

[2026-09-01T10:00:00Z] > products
  BTC/USDT
  ETH/USDT

[2026-09-01T10:00:00Z] > stats BTC/USDT
Bids: 2, asks: 2
Highest bid: 59800.000000
Lowest ask:  60000.000000

[2026-09-01T10:00:00Z] > wallet
BTC: 1.000000
ETH: 8.000000
USDT: 15000.000000
```

## Commands

| Command | Purpose |
|---|---|
| `help` | Show command help |
| `products` | List trading pairs |
| `stats <SYMBOL>` | Show current bid/ask statistics |
| `buy <SYMBOL> <PRICE> <QTY>` | Place a limit buy |
| `sell <SYMBOL> <PRICE> <QTY>` | Place a limit sell |
| `wallet` | Show simulated balances |
| `step` | Match current orders and advance time |
| `trades` | Show simulated-user trades |
| `quit` | Exit |

## Possible next improvements

- Reserve wallet funds when an order is placed instead of checking only at submission time
- Add cancellation and order-status tracking
- Support market orders
- Persist trade history to disk
- Add richer unit tests and property-based edge-case testing
- Add benchmark tests for larger order books
- Add a small terminal UI or web visualization

## Disclaimer

This program is an educational simulator. It is not a real exchange, trading bot, brokerage system, or financial recommendation engine.
