#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <functional>
#include <concepts>
#include <vector>
#include <memory>
#include <fstream>
#include <ctime>
#include <format>

template <typename F>
concept Action = requires(F f) {
    { f() } -> std::same_as<void>;
};

class IAction {
public:
    virtual void execute() = 0;
    virtual ~IAction() = default;
};

void logToFile(const std::string& message) {
    std::ofstream logFile("malware_prevent.log", std::ios::app); // Open in append mode
    if (logFile.is_open()) {
        std::time_t now = std::time(nullptr);
        std::string timeStr = std::ctime(&now);
        timeStr.pop_back(); // Remove newline from ctime

        logFile << "[" << timeStr << "] " << message << std::endl;
    } else {
        std::cerr << "Failed to open log file!" << std::endl;
    }
}

class KillProcessAction : public IAction {
private:
    std::string processName;

public:
    explicit KillProcessAction(const std::string& name) : processName(name) {}

    void execute() override {
        std::string command = "pgrep " + processName;
        std::string result = exec(command.c_str());

        if (!result.empty()) {
            std::print("Process '{}' found. Killing process...\n", processName);
            std::string killCommand = "pkill " + processName;
            system(killCommand.c_str());

            logToFile(std::format("Process '{}' found and killed.", processName));
        } else {
            std::print("Process '{}' not found.\n", processName);
        }
    }

private:
    std::string exec(const char* cmd) {
        char buffer[128];
        std::string result = "";
        FILE* pipe = popen(cmd, "r");
        if (!pipe) throw std::runtime_error("popen() failed!");
        try {
            while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
                result += buffer;
            }
        } catch (...) {
            pclose(pipe);
            throw;
        }
        pclose(pipe);
        return result;
    }
};

class RemoveDirectoryAction : public IAction {
private:
    std::string directoryPath;

public:
    explicit RemoveDirectoryAction(const std::string& path) : directoryPath(path) {}

    void execute() override {
        std::string removeCommand = "rm -rf " + directoryPath;
        std::print("Removing directory '{}'...\n", directoryPath);
        int result = system(removeCommand.c_str());

        if (result != 0) {
            logToFile(std::format("Directory '{}' removed.", directoryPath));
        }
    }
};

class Monitor {
private:
    std::vector<std::unique_ptr<IAction>> actions;
    int intervalSeconds;

public:
    explicit Monitor(int interval) : intervalSeconds(interval) {}

    void addAction(std::unique_ptr<IAction> action) {
        actions.push_back(std::move(action));
    }

    void start() {
        std::print("Starting monitor with interval {} seconds...\n", intervalSeconds);
        while (true) {
            for (const auto& action : actions) {
                action->execute();
            }
            sleep(intervalSeconds);
        }
    }
};

class PrintMessageAction : public IAction {
private:
    std::string message;

public:
    explicit PrintMessageAction(const std::string& msg) : message(msg) {}

    void execute() override {
        std::print("Message: {}\n", message);
    }
};

auto main() -> int {

    Monitor monitor(5);

    monitor.addAction(std::make_unique<PrintMessageAction>("Analyzing process.."));
    monitor.addAction(std::make_unique<KillProcessAction>("perfctl"));
    monitor.addAction(std::make_unique<RemoveDirectoryAction>("/root/.config/cron/perfcc"));
    monitor.addAction(std::make_unique<RemoveDirectoryAction>("/usr/bin/perfcc"));
    monitor.addAction(std::make_unique<RemoveDirectoryAction>("/usr/lib/libfsnkdev.so"));
    monitor.addAction(std::make_unique<RemoveDirectoryAction>("/var/www/.config/cron/perfcc"));

    monitor.start();

    return 0;
}
