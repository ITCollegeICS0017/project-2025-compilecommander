#include "exchange_manager.h"
#include "currency_repository.h"
#include "transaction.h"
#include "iclock.h"
#include "report.h"
#include <stdexcept>
#include <sstream>

ExchangeManager::ExchangeManager(ICurrencyRepository& repo, IClock& clock, Report& report)
    : repo_(repo), clock_(clock), report_(report) {}

Transaction ExchangeManager::processExchange(const std::string& txId,
                                             Currency fromC, double fromA,
                                             Currency toC) {
    if (fromA < 0.0) throw std::invalid_argument("amount must be >= 0");
    const double rate = repo_.getRate(fromC, toC);
    const double payout = fromA * rate;

    // Validate reserve
    if (repo_.getBalance(toC) < payout) {
        std::ostringstream msg;
        msg << "Insufficient reserve of " << to_string(toC) << " to pay out " << payout;
        throw std::runtime_error(msg.str());
    }

    // Create transaction and apply
    auto t = Transaction::make(txId, clock_.now_iso8601(), fromC, fromA, toC, rate);
    repo_.applyExchange(fromC, fromA, toC, payout);
    report_.add(t);
    return t;
}
