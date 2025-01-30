# PerfctlKiller

`PerfctlKiller` is a C++ application designed to monitor and perform security actions, including killing malicious processes and removing suspicious directories or files. The program runs on a defined interval and executes a series of actions that are customizable. This tool is useful for preventing or mitigating malicious activities on a Linux system.

## Features

- **Process Monitoring**: Kills malicious processes based on the process name.
- **Directory Removal**: Removes potentially harmful directories or files.
- **Logging**: Logs actions and their results to a log file for tracking and auditing.
- **Customizable Actions**: Easily extendable for other actions (e.g., system checks, cleanup, etc.).
- **Interval-based Execution**: Actions are executed periodically on a set interval.

## Requirements

- CMake 3.16 or higher
- C++23 Standard
- Linux-based Operating System (tested on Ubuntu)
- A compatible C++ compiler (e.g., GCC, Clang)

## Installation

### Clone the repository:

```bash
git clone https://github.com/thecompez/PerfctlKiller.git
cd PerfctlKiller
```

### Build the project:

Make sure you have [CMake](https://cmake.org/install/) installed on your system.

```bash
mkdir build
cd build
cmake ..
make
```

### Install the program:

```bash
sudo make install
```

This will install the executable to the default system directories (`/usr/bin`, `/usr/lib`, etc.).

## Usage

To run the application, execute the compiled binary:

```bash
PerfctlKiller
```

The program will continuously monitor and perform the defined actions at the specified interval (default is every 5 seconds). 

You can modify the list of actions inside the `main.cpp` file to include your own processes or directories to monitor.

## Actions

- **PrintMessageAction**: Prints a custom message to the console (useful for logging or monitoring purposes).
- **KillProcessAction**: Kills a specified process by name. The process is identified using `pgrep` and killed with `pkill`.
- **RemoveDirectoryAction**: Removes a specified directory and its contents using the `rm -rf` command.
  
### Example Configuration

The `main.cpp` file can be modified to add new actions. Here’s how you might add more actions:

```cpp
monitor.addAction(std::make_unique<PrintMessageAction>("Custom action running"));
monitor.addAction(std::make_unique<KillProcessAction>("malicious-process"));
monitor.addAction(std::make_unique<RemoveDirectoryAction>("/path/to/suspicious/folder"));
```

## Logging

All actions are logged in a file named `malware_prevent.log` in the current directory. The log includes timestamps for when actions are performed, making it easy to track activity.

### Example Log Entry:

```
[Wed Jan 30 13:42:01 2025] Process 'perfctl' found and killed.
[Wed Jan 30 13:42:06 2025] Directory '/root/.config/cron/perfcc' removed.
```

## Contributing

Contributions are welcome! If you'd like to contribute, please fork the repository, create a new branch, and submit a pull request.

### Steps to contribute:

1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Commit your changes.
4. Push your changes to your fork.
5. Submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- The idea for this project came from a need to improve system security by automating the detection and removal of malicious processes and files.
