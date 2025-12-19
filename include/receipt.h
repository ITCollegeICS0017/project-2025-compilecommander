#pragma once
#include <iosfwd>
class Transaction;

struct Receipt {
    static void print(const Transaction& t, std::ostream& os);
    static std::string csv(const Transaction& t);
};
