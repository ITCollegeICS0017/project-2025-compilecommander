#pragma once
#include <map>
#include "currency.h"

// Data Layer Abstraction
class ICurrencyRepository {
public:
    virtual ~ICurrencyRepository() = default;
    virtual double getRate(Currency from, Currency to) const = 0;
    virtual double getBalance(Currency c) const = 0;
    virtual void   setBalance(Currency c, double amount) = 0;
    virtual void   applyExchange(Currency fromC, double fromA,
                                 Currency toC,   double toA) = 0;
};

// Concrete In‑Memory Repository
class CurrencyRepository : public ICurrencyRepository {
public:
    explicit CurrencyRepository(double lowThreshold = 100.0);

    // ICurrencyRepository
    double getRate(Currency from, Currency to) const override;
    double getBalance(Currency c) const override;
    void   setBalance(Currency c, double amount) override;
    void   applyExchange(Currency fromC, double fromA,
                         Currency toC,   double toA) override;

    // extra read‑only helper
    double low_threshold() const { return low_threshold_; }
    bool   is_low(Currency c) const { return getBalance(c) < low_threshold_; }

    // Configure default sample rates (1:1 except explicit pairs)
    void seedSampleRates();

private:
    std::map<Currency,double> balances_{};
    std::map<std::pair<Currency,Currency>, double> rates_{};
    double low_threshold_{};
};
