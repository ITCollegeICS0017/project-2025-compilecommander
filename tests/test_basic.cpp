#include <iostream>
#include <stdexcept>

// Minimal headers from your src/ (adjust include paths as needed)
#include "../src/currency_reserve.h"
#include "../src/transaction.h"
#include "../src/cashier.h"
#include "../src/report.h"
#include "../src/receipt.h"

int main() {
    CurrencyReserve reserve(100.0);
    reserve.set_balance(Currency::EUR, 1000.0);
    reserve.set_balance(Currency::USD,  500.0);

    Report report;
    Cashier cashier{"T-001", "TestUser", reserve, report};

    // Should succeed
    auto tx = cashier.exchange("T-OK", Currency::EUR, 100.0, Currency::USD, 1.1);
    if (tx.to_amount != 110.0) {
        std::cerr << "Expected to_amount 110.0, got " << tx.to_amount << "\n";
        return 1;
    }

    // Should fail (insufficient)
    try {
        reserve.set_balance(Currency::USD, 50.0);
        (void)cashier.exchange("T-FAIL", Currency::EUR, 100.0, Currency::USD, 1.1);
        std::cerr << "Expected failure but succeeded\n";
        return 2;
    } catch (const std::exception&) {
        // expected
    }

    std::cout << "test_basic OK\n";
    return 0;
}
