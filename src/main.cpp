#include <iostream>
#include "iclock.h"
#include "currency_repository.h"
#include "exchange_manager.h"
#include "console_ui.h"
#include "report.h"

int main() {
    // === Data Layer ===
    CurrencyRepository repo(/*lowThreshold=*/100.0);
    repo.setBalance(Currency::EUR, 1000.0);
    repo.setBalance(Currency::USD, 500.0);
    repo.setBalance(Currency::GBP, 300.0);
    repo.setBalance(Currency::SEK, 1000.0);
    repo.setBalance(Currency::DKK, 1000.0);

    // === Shared Services ===
    RealClock clock;
    Report report;

    // === Logic Layer ===
    ExchangeManager manager(repo, clock, report);

    // === UI Layer ===
    ConsoleUI ui(manager);
    ui.run();  // interactive session

    // === End-of-day summary ===
    report.print_summary(std::cout, repo);

    return 0;
}
