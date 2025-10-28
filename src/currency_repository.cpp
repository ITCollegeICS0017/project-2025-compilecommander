#include "currency_repository.h"
#include <stdexcept>

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
