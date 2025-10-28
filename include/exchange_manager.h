#pragma once
#include <string>
#include "currency.h"
#include "transaction.h"   // ← add this

class ICurrencyRepository;
class IClock;
class Report;

class ExchangeManager {
    ICurrencyRepository& repo_;
    IClock& clock_;
    Report& report_;

public:
    ExchangeManager(ICurrencyRepository& repo, IClock& clock, Report& report);

    Transaction processExchange(const std::string& txId,
                                Currency fromC, double fromA,
                                Currency toC);

    // ✅ New accessors for UI commands
    const Report& report() const { return report_; }
    const ICurrencyRepository& repo() const { return repo_; }
};