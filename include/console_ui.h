#pragma once
#include "currency.h"
class ExchangeManager;

class ConsoleUI {
public:
    explicit ConsoleUI(ExchangeManager& mgr) : mgr_(mgr) {}
    void run();

private:
    ExchangeManager& mgr_;
    void showMenu() const;
    static Currency parseCurrency(const std::string& s);
};
