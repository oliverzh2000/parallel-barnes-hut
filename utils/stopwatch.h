//
// Created by Oliver Zhang on 2020-01-18.
//

#ifndef BH_SIM_STOPWATCH_H
#define BH_SIM_STOPWATCH_H

#include <chrono>
#include <string>

class Stopwatch final {
    std::string name;
    std::chrono::time_point<std::chrono::high_resolution_clock , std::chrono::duration<double>> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock , std::chrono::duration<double>> stop_time;
    static bool doOutput;

public:
    /// Create a new Stopwatch and call start on it right before returning.
    static Stopwatch createAndStart(std::string name);

    /// Stop this Stopwatch and output its time elapsed if doOutput is set to true.
    void stopAndOutput();

    /// Sets the doOutput global flag (applies to all Stopwatch instances).
    /// If true, then stopAndOutput will print elpased time. Otherwise, stopAndOutput will print nothing.
    void static setOutput(bool doOutput);

private:

    explicit Stopwatch(std::string name);
    void start();
    void stop();
    void output() const;
};


#endif //BH_SIM_STOPWATCH_H
