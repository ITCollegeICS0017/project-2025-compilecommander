#include "transaction.h"
#include <stdexcept>

Transaction Transaction::make(const std::string& id,
                              const std::string& timestamp,
                              Currency fromC, double fromA,
                              Currency toC,   double rate) {
    Transaction t;
    t.set_id(id);
    t.set_timestamp(timestamp);
    t.set_from(fromC, fromA);
    t.set_rate(rate);
    t.set_to(toC, fromA * rate);
    return t;
}

void Transaction::set_id(const std::string& v) {
    if (v.empty()) throw std::invalid_argument("tx id must not be empty");
    tx_id_ = v;
}
void Transaction::set_timestamp(const std::string& v) {
    if (v.empty()) throw std::invalid_argument("timestamp must not be empty");
    timestamp_ = v;
}
void Transaction::set_from(Currency c, double amount) {
    if (amount < 0.0) throw std::invalid_argument("from_amount must be >= 0");
    from_currency_ = c;
    from_amount_   = amount;
}
void Transaction::set_to(Currency c, double amount) {
    if (amount < 0.0) throw std::invalid_argument("to_amount must be >= 0");
    to_currency_ = c;
    to_amount_   = amount;
}
void Transaction::set_rate(double r) {
    if (r <= 0.0) throw std::invalid_argument("rate must be > 0");
    rate_ = r;
}
