#include "receipt.h"
#include "transaction.h"
#include "currency.h"
#include <iostream>
#include <sstream>

void Receipt::print(const Transaction& t, std::ostream& os) {
    os << "----- RECEIPT -----\n"
       << "Receipt/Tx ID: " << t.id() << "\n"
       << "Timestamp:     " << t.timestamp() << "\n"
       << "From:          " << to_string(t.from_currency())
       << " " << t.from_amount() << "\n"
       << "To:            " << to_string(t.to_currency())
       << " " << t.to_amount() << "\n"
       << "Rate:          " << t.rate() << "\n"
       << "-------------------\n";
}

std::string Receipt::csv(const Transaction& t) {
    std::ostringstream oss;
    oss << t.timestamp() << ','
        << t.id() << ','
        << to_string(t.from_currency()) << ','
        << t.from_amount() << ','
        << to_string(t.to_currency()) << ','
        << t.to_amount() << ','
        << t.rate();
    return oss.str();
}
