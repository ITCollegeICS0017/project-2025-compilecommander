#include "iclock.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

std::string RealClock::now_iso8601() const {
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t now_c = system_clock::to_time_t(now);

    std::tm tm_buf{};
#if defined(_WIN32) && !defined(__MINGW32__)
    // MSVC (Visual Studio)
    localtime_s(&tm_buf, &now_c);
#elif defined(__MINGW32__) || defined(__MINGW64__)
    // MinGW (GNU on Windows)
    tm_buf = *std::localtime(&now_c);
#else
    // Linux/macOS
    localtime_r(&now_c, &tm_buf);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%dT%H:%M:%S");

    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    oss << '.' << std::setw(3) << std::setfill('0') << ms.count() << 'Z';

    return oss.str();
}
