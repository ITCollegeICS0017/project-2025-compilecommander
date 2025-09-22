// main.cpp  —  Release 1 prototype (C++17)
// Console-only. No external libs.

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <sstream>
#include <stdexcept>

//
// ======= Domain Basics =======
//

enum class Currency { EUR, USD, GBP, SEK, DKK };

static const char* to_string(Currency c) {
    switch (c) {
        case Currency::EUR: return "EUR";
        case Currency::USD: return "USD";
        case Currency::GBP: return "GBP";
        case Currency::SEK: return "SEK";
        case Currency::DKK: return "DKK";
    }
    return "?";
}

std::string now_iso8601() {
    using clock = std::chrono::system_clock;
    auto t = clock::to_time_t(clock::now());
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return oss.str();
}

//
// ======= Structs =======
//

struct Transaction {
    std::string tx_id;
    std::string timestamp;      // ISO 8601 for simplicity
    Currency from_currency{};
    double   from_amount{};
    Currency to_currency{};
    double   to_amount{};       // computed
    double   rate{};            // from -> to

    static Transaction make(std::string id,
                            Currency fromC, double fromA,
                            Currency toC,   double rate) {
        Transaction t;
        t.tx_id         = std::move(id);
        t.timestamp     = now_iso8601();
        t.from_currency = fromC;
        t.from_amount   = fromA;
        t.to_currency   = toC;
        t.rate          = rate;
        t.to_amount     = fromA * rate;      // simple placeholder calc
        return t;
    }
};

//
// ======= Classes =======
//

class CurrencyReserve {
public:
    explicit CurrencyReserve(double lowThreshold = 100.0)
        : low_threshold_(lowThreshold) {}

    void set_balance(Currency c, double amount) {
        balances_[c] = amount;
    }

    double get_balance(Currency c) const {
        auto it = balances_.find(c);
        return (it == balances_.end()) ? 0.0 : it->second;
    }

    // Validate we can pay out 'to_amount' and accept 'from_amount'.
    bool can_fulfill(const Transaction& t) const {
        // We need to have at least t.to_amount in the target currency
        return get_balance(t.to_currency) >= t.to_amount;
    }

    // Apply effects of a completed transaction:
    // - Office receives 'from_currency' (increase)
    // - Office pays out 'to_currency' (decrease)
    void apply(const Transaction& t) {
        balances_[t.from_currency] += t.from_amount;
        balances_[t.to_currency]   -= t.to_amount;
    }

    bool is_low(Currency c) const {
        return get_balance(c) < low_threshold_;
    }

    double low_threshold() const { return low_threshold_; }

private:
    std::map<Currency, double> balances_;
    double low_threshold_;
};

class Receipt {
public:
    static void print(const Transaction& t, std::ostream& os = std::cout) {
        os << "----- RECEIPT -----\n"
           << "Receipt/Tx ID: " << t.tx_id << "\n"
           << "Timestamp:     " << t.timestamp << "\n"
           << "From:          " << to_string(t.from_currency)
           << " " << t.from_amount << "\n"
           << "To:            " << to_string(t.to_currency)
           << " " << t.to_amount  << "\n"
           << "Rate:          " << t.rate << "\n"
           << "-------------------\n";
    }

    // Minimal CSV line (placeholder for logging)
    static std::string csv(const Transaction& t) {
        std::ostringstream oss;
        oss << t.timestamp << ','
            << t.tx_id << ','
            << to_string(t.from_currency) << ','
            << t.from_amount << ','
            << to_string(t.to_currency) << ','
            << t.to_amount << ','
            << t.rate;
        return oss.str();
    }
};

class Report {
public:
    void add(const Transaction& t) {
        transactions_.push_back(t);
        // Very naive "profit": EUR received minus USD paid converted back, etc.
        // In a real system you'd have buy/sell spread, fees, etc.
        // Placeholder: assume profit encoded elsewhere. Keep 0 here.
    }

    void print_summary(const CurrencyReserve& reserve,
                       std::ostream& os = std::cout) const {
        os << "=== DAILY REPORT (prototype) ===\n";
        os << "Total transactions: " << transactions_.size() << "\n";
        os << "Closing balances (sample currencies):\n";
        for (Currency c : {Currency::EUR, Currency::USD, Currency::GBP, Currency::SEK, Currency::DKK}) {
            os << "  " << to_string(c) << ": " << reserve.get_balance(c);
            if (reserve.is_low(c)) os << "  [LOW]";
            os << "\n";
        }
        os << "===============================\n";
    }

private:
    std::vector<Transaction> transactions_;
};

class Cashier {
public:
    Cashier(std::string id, std::string name, CurrencyReserve& reserve, Report& report)
        : id_(std::move(id)), name_(std::move(name)),
          reserve_(reserve), report_(report) {}

    // Perform an exchange; returns the finalized Transaction.
    // Throws std::runtime_error if reserve cannot fulfill payout.
    Transaction exchange(const std::string& txId,
                         Currency fromC, double fromA,
                         Currency toC,   double rate) {
        Transaction t = Transaction::make(txId, fromC, fromA, toC, rate);

        // 1) Validate reserve before confirming
        if (!reserve_.can_fulfill(t)) {
            std::ostringstream msg;
            msg << "Insufficient reserve of " << to_string(t.to_currency)
                << " to pay out " << t.to_amount;
            throw std::runtime_error(msg.str());
        }

        // 2) Apply to reserves (atomicity/rollback not modeled in this prototype)
        reserve_.apply(t);

        // 3) Log into report (placeholder aggregation)
        report_.add(t);

        // 4) Notify low reserve (prototype: just print)
        if (reserve_.is_low(t.to_currency)) {
            std::cout << "[WARN] Low reserve in " << to_string(t.to_currency)
                      << " (threshold " << reserve_.low_threshold() << ")\n";
        }

        return t;
    }

    void print_receipt(const Transaction& t) const {
        Receipt::print(t);
        // Placeholder "append CSV" (would write to file in a real system)
        std::cout << "CSV: " << Receipt::csv(t) << "\n";
    }

private:
    std::string id_;
    std::string name_;
    CurrencyReserve& reserve_;
    Report& report_;
};

//
// ======= Demo (also covers Step 5 scenario, but keep if you only need Step 4) =======
//

int main() {
    // Setup initial reserves (manager prepared before opening)
    CurrencyReserve reserve(/*lowThreshold=*/100.0);
    reserve.set_balance(Currency::EUR, 1000.0);
    reserve.set_balance(Currency::USD,  50.0);   // intentionally low to trigger a failure first
    reserve.set_balance(Currency::GBP,  300.0);
    reserve.set_balance(Currency::SEK, 1000.0);
    reserve.set_balance(Currency::DKK, 1000.0);

    Report report;
    Cashier cashier{"C-001", "Alice", reserve, report};

    // === User Story demo ===
    // US-1: Cashier enters details; system validates reserves and converts.
    // Try a transaction that should FAIL (insufficient USD).
    const double EUR_to_USD = 1.10;
    try {
        auto txFail = cashier.exchange("TX-FAIL",
                                       Currency::EUR, 100.0,
                                       Currency::USD, EUR_to_USD);
        (void)txFail; // not reached
    } catch (const std::exception& ex) {
        std::cerr << "[EXPECTED] Rejected: " << ex.what() << "\n";
    }

    // Manager tops up USD reserve during the day.
    reserve.set_balance(Currency::USD, 500.0);

    // Try a transaction that should SUCCEED.
    try {
        auto txOk = cashier.exchange("TX-0001",
                                     Currency::EUR, 100.0,
                                     Currency::USD, EUR_to_USD);

        // US-2: Print receipt for the client; CSV shown as placeholder.
        cashier.print_receipt(txOk);
    } catch (const std::exception& ex) {
        std::cerr << "[ERROR] " << ex.what() << "\n";
    }

    // US-3: End-of-day report for the manager.
    report.print_summary(reserve);
    return 0;
}