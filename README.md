# Deribit Trading System

A high-performance order execution and management system for trading on Deribit Test (https://test.deribit.com/) implemented in C++.

## Overview

This system provides a comprehensive trading infrastructure for connecting to Deribit's test environment, managing orders, and handling real-time market data. It's designed with a focus on low latency and reliability.

## Features

### Order Management
- Place new orders (market/limit)
- Cancel existing orders
- Modify active orders
- Track current positions
- View orderbook data

### Market Data
- Real-time market data streaming via WebSocket
- Order book updates
- Support for multiple instruments
- Market depth tracking

### Supported Instruments
- Spot trading
- Futures contracts
- Options trading

### Technical Features
- Low-latency architecture
- Thread-safe operations
- Comprehensive error handling
- Detailed system logging
- WebSocket server for client connections

## Requirements

### System Requirements
- C++17 compatible compiler
- CMake 3.14 or higher
- OpenSSL
- Boost libraries
- WebSocket++
- nlohmann/json

### Build Dependencies
```bash
# Ubuntu/Debian
sudo apt-get install libssl-dev libboost-all-dev cmake build-essential

# MacOS
brew install openssl boost cmake

# Windows
vcpkg install openssl:x64-windows boost:x64-windows
```

## Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Configuration

1. Create a Deribit Test account at https://test.deribit.com/
2. Generate API keys in your account settings
3. Configure the system:
   ```bash
   cp config.example.json config.json
   # Edit config.json with your API keys and preferences
   ```

## Usage

### Basic Order Placement
```cpp
#include "trading/order_manager.hpp"

int main() {
    OrderManager manager;
    
    // Place a limit order
    Order order = manager.place_order("BTC-PERPETUAL", 50000.0, 1.0, true);
    
    // Check order status
    OrderStatus status = manager.get_order_status(order.id);
}
```

### Market Data Subscription
```cpp
#include "market_data/market_data_handler.hpp"

int main() {
    MarketDataHandler handler;
    
    // Subscribe to orderbook updates
    handler.subscribe("BTC-PERPETUAL");
    
    // Get current orderbook
    OrderBook book = handler.get_orderbook("BTC-PERPETUAL");
}
```

## Project Structure

```
├── include/          # Header files
├── src/             # Source files
├── tests/           # Unit and integration tests
├── examples/        # Usage examples
└── docs/            # Documentation
```

## Testing

Run the test suite:
```bash
cd build
ctest
```

## Logging

The system uses spdlog for logging. Logs are written to:
- `logs/system.log`: General system logs
- `logs/orders.log`: Order-related events
- `logs/market_data.log`: Market data updates

## Error Handling

The system implements comprehensive error handling:
- Network connection issues
- API errors
- Invalid order parameters
- Market data inconsistencies

## Performance Considerations

The system is optimized for:
- Minimal order placement latency
- Efficient market data processing
- Optimal memory usage
- Thread management
- Network communication efficiency

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Disclaimer

This trading system is for educational and testing purposes only. Do not use it for real trading without proper testing and risk management.

## Contact

For questions or support, please contact:
- Email: anthony@goquant.io

## Acknowledgments

- Deribit API documentation
- WebSocket++ library
- nlohmann/json library
