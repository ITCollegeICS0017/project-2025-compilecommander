#pragma once
#include <vector>
#include "currency.h"
class Transaction;

class Report {
public:
    void add(const Transaction& t);
    void print_summary(std::ostream& os,
                       const class ICurrencyRepository& repo) const;
private:
    std::vector<Transaction> tx_;
};
