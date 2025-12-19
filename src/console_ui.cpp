#include "console_ui.h"
#include "exchange_manager.h"
#include "transaction.h"
#include "receipt.h"
#include "report.h"               // ← needed to call Report::print_summary
#include "currency_repository.h"  // ← needed because print_summary takes ICurrencyRepository&
#include <iostream>
#include <sstream>
#include <stdexcept>

void ConsoleUI::showMenu() const {
    std::cout << "\n=== Currency Exchange ===\n";
    std::cout << "Format: FROM AMOUNT TO (e.g., EUR 100 USD)\n";
    std::cout << "Type 'quit' to exit.\n> ";
}

static std::string upper(std::string s){ for(char& c: s) c = (char)std::toupper((unsigned char)c); return s; }

Currency ConsoleUI::parseCurrency(const std::string& s) {
    const std::string u = upper(s);
    if (u == "EUR") return Currency::EUR;
    if (u == "USD") return Currency::USD;
    if (u == "GBP") return Currency::GBP;
    if (u == "SEK") return Currency::SEK;
    if (u == "DKK") return Currency::DKK;
    throw std::invalid_argument("unknown currency: " + s);
}

void ConsoleUI::run() {
    std::string fromS, toS;
    double amount;

    while (true) {
        std::cout << "\n=== Currency Exchange ===\n";
        std::cout << "Format: FROM AMOUNT TO (e.g., EUR 100 USD)\n";
        std::cout << "Type 'help' to view commands or 'quit' to exit.\n> ";

        std::string line;
        if (!std::getline(std::cin, line))
            break;

        // Trim spaces
        if (line.empty()) {
            std::cout << "Type 'help' for usage instructions.\n";
            continue;
        }

        std::istringstream iss(line);
        if (!(iss >> fromS)) continue;

        // Commands
        if (fromS == "quit" || fromS == "exit")
            break;

        if (fromS == "help") {
    std::cout << "\n=== Help Menu ===\n"
              << "Commands:\n"
              << "  help          - Show this help menu\n"
              << "  quit / exit   - Exit the program\n"
              << "  report        - Show all transactions and balances\n"
              << "  EUR 100 USD   - Convert 100 EUR to USD\n\n"
              << "Supported currencies: EUR, USD, GBP, SEK, DKK\n";
    continue;
}

if (fromS == "report" || fromS == "print") {
    std::cout << "\n=== Report ===\n";
    mgr_.report().print_summary(std::cout, mgr_.repo());
    continue;
}

        // Try parse normal input (FROM AMOUNT TO)
        if (!(iss >> amount >> toS)) {
            std::cout << "Invalid input. Try again or type 'help'.\n";
            continue;
        }

        try {
            auto t = mgr_.processExchange("TX-USER", parseCurrency(fromS), amount, parseCurrency(toS));
            Receipt::print(t, std::cout);
            std::cout << "CSV: " << Receipt::csv(t) << "\n";
        } catch (const std::exception &ex) {
            std::cout << "Error: " << ex.what() << "\n";
        }
    }

    std::cout << "Exiting Currency Exchange. Goodbye!\n";
}
