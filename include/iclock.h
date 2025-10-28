#pragma once
#include <string>

struct IClock {
    virtual ~IClock() = default;
    virtual std::string now_iso8601() const = 0;
};

struct RealClock : public IClock {
    std::string now_iso8601() const override;
};
