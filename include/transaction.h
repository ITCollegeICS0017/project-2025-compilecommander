#pragma once
#include <string>
#include "currency.h"

class Transaction {
public:
    // Factory for a valid transaction (rate must be > 0, amount >= 0)
    static Transaction make(const std::string& id,
                            const std::string& timestamp,
                            Currency fromC, double fromA,
                            Currency toC,   double rate);

    // Getters
    const std::string& id() const { return tx_id_; }
    const std::string& timestamp() const { return timestamp_; }
    Currency from_currency() const { return from_currency_; }
    double   from_amount()   const { return from_amount_; }
    Currency to_currency()   const { return to_currency_; }
    double   to_amount()     const { return to_amount_; }
    double   rate()          const { return rate_; }

private:
    std::string tx_id_;
    std::string timestamp_;
    Currency from_currency_{};
    double   from_amount_{};
    Currency to_currency_{};
    double   to_amount_{};
    double   rate_{};

    // Private setters enforce validation
    void set_id(const std::string& v);
    void set_timestamp(const std::string& v);
    void set_from(Currency c, double amount);
    void set_to(Currency c, double amount);
    void set_rate(double r);
};
