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

    // Release 4: File persistence
    void loadAll(const std::string& filename);
    void saveAll(const std::string& filename) const;

private:
    std::map<Currency,double> balances_{};
    std::map<std::pair<Currency,Currency>, double> rates_{};
    double low_threshold_{};

    // Helper methods for file parsing/formatting
    bool parseLine(const std::string& line, Currency& curr, double& balance);
    std::string toLine(Currency curr, double balance) const;
};
