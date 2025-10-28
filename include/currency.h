#pragma once
#include <string>

enum class Currency { EUR, USD, GBP, SEK, DKK };

inline const char* to_string(Currency c) {
    switch (c) {
        case Currency::EUR: return "EUR";
        case Currency::USD: return "USD";
        case Currency::GBP: return "GBP";
        case Currency::SEK: return "SEK";
        case Currency::DKK: return "DKK";
    }
    return "?";
}
