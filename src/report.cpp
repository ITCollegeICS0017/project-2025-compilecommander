#include "report.h"
#include "currency.h"
#include "transaction.h"
#include "currency_repository.h"
#include <iostream>

void Report::add(const Transaction& t) {
    tx_.push_back(t);
}

void Report::print_summary(std::ostream& os, const ICurrencyRepository& repo) const {
    os << "=== DAILY REPORT ===\n";
    os << "Total transactions: " << tx_.size() << "\n";
    os << "Closing balances:\n";
    for (Currency c : {Currency::EUR, Currency::USD, Currency::GBP, Currency::SEK, Currency::DKK}) {
        os << "  " << to_string(c) << ": " << repo.getBalance(c) << "\n";
    }
    os << "====================\n";
}
