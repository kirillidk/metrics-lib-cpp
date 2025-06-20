#pragma once

#include <chrono>       // std::chrono::seconds
#include <fstream>      // std::ofstream
#include <memory>       // std::shared_ptr
#include <string>       // std::string
#include <string_view>  // std::string_view
#include <thread>       // std::jthread

namespace Metrics {

// forward declaration
class Registry;

class Dumper : public std::enable_shared_from_this<Dumper> {
private:
    std::ofstream m_os;
    const std::string m_filename;
    std::jthread m_worker;
public:
    Dumper(std::string_view filename) : m_filename(filename) {
        m_os.open(std::string(filename));
    }

    ~Dumper() {
        disableAutoWrite();
        if (m_os.is_open()) m_os.close();
    }

    void write(std::shared_ptr<Metrics::Registry> registry);
    void enableAutoWrite(
        std::shared_ptr<Metrics::Registry> registry,
        std::chrono::seconds interval
    );
    void disableAutoWrite();
    void reset() {
        disableAutoWrite();
        if (m_os.is_open()) m_os.close();
    }
};

std::string getCurrentTimestamp();

}  // namespace Metrics