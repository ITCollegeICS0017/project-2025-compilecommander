#include "currency_repository.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>

namespace {
    inline std::pair<Currency,Currency> key(Currency a, Currency b) { return {a,b}; }
}

CurrencyRepository::CurrencyRepository(double lowThreshold)
    : low_threshold_(lowThreshold) {
    // Default zero balances
    balances_[Currency::EUR] = 0.0;
    balances_[Currency::USD] = 0.0;
    balances_[Currency::GBP] = 0.0;
    balances_[Currency::SEK] = 0.0;
    balances_[Currency::DKK] = 0.0;

    seedSampleRates();
}

void CurrencyRepository::seedSampleRates() {
    // Identity rates
    for (Currency a : {Currency::EUR, Currency::USD, Currency::GBP, Currency::SEK, Currency::DKK}) {
        rates_[key(a,a)] = 1.0;
    }
    // Simple sample pairs
    rates_[key(Currency::EUR, Currency::USD)] = 1.10;
    rates_[key(Currency::USD, Currency::EUR)] = 0.91;
    rates_[key(Currency::EUR, Currency::GBP)] = 0.85;
    rates_[key(Currency::GBP, Currency::EUR)] = 1.18;
}

double CurrencyRepository::getRate(Currency from, Currency to) const {
    auto it = rates_.find(key(from,to));
    if (it == rates_.end()) throw std::runtime_error("rate not available for pair");
    return it->second;
}

double CurrencyRepository::getBalance(Currency c) const {
    auto it = balances_.find(c);
    return (it == balances_.end()) ? 0.0 : it->second;
}

void CurrencyRepository::setBalance(Currency c, double amount) {
    balances_[c] = amount;
}

void CurrencyRepository::applyExchange(Currency fromC, double fromA,
                                       Currency toC,   double toA) {
    balances_[fromC] += fromA;   // office receives source currency
    balances_[toC]   -= toA;     // office pays out target currency
}

// ============================================================================
// Release 4: File Persistence Implementation
// ============================================================================

void CurrencyRepository::loadAll(const std::string& filename) {
    std::ifstream file(filename);

    // If file doesn't exist, start with empty repository (valid state)
    if (!file.is_open()) {
        std::cout << "[INFO] No existing data file found. Starting with empty balances.\n";
        return;
    }

    std::string line;
    int line_number = 0;
    int valid_count = 0;
    int invalid_count = 0;

    while (std::getline(file, line)) {
        line_number++;

        // Skip empty lines
        if (line.empty()) continue;

        Currency curr;
        double balance;

        // Try to parse the line
        if (parseLine(line, curr, balance)) {
            balances_[curr] = balance;
            valid_count++;
        } else {
            // Invalid line - skip silently (no exceptions)
            invalid_count++;
            std::cout << "[WARN] Skipping invalid line " << line_number
                      << ": " << line << "\n";
        }
    }

    file.close();

    std::cout << "[INFO] Loaded " << valid_count << " currency balances";
    if (invalid_count > 0) {
        std::cout << " (skipped " << invalid_count << " invalid lines)";
    }
    std::cout << "\n";
}

void CurrencyRepository::saveAll(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    // Write all currency balances
    for (const auto& [curr, balance] : balances_) {
        file << toLine(curr, balance) << "\n";
    }

    file.close();
    std::cout << "[INFO] Saved " << balances_.size() << " currency balances to "
              << filename << "\n";
}

bool CurrencyRepository::parseLine(const std::string& line, Currency& curr, double& balance) {
    std::istringstream iss(line);
    std::string currCode;

    // Expected format: "CUR;balance"
    // Example: "EUR;1000.0"

    if (!std::getline(iss, currCode, ';')) {
        return false;
    }

    if (!(iss >> balance)) {
        return false;
    }

    // Validate balance is non-negative
    if (balance < 0.0) {
        return false;
    }

    // Convert string to Currency enum
    if (currCode == "EUR") {
        curr = Currency::EUR;
    } else if (currCode == "USD") {
        curr = Currency::USD;
    } else if (currCode == "GBP") {
        curr = Currency::GBP;
    } else if (currCode == "SEK") {
        curr = Currency::SEK;
    } else if (currCode == "DKK") {
        curr = Currency::DKK;
    } else {
        return false; // Unknown currency code
    }

    return true;
}

std::string CurrencyRepository::toLine(Currency curr, double balance) const {
    std::ostringstream oss;
    oss << to_string(curr) << ";" << balance;
    return oss.str();
}
