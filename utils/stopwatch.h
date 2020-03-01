//
// Created by Oliver Zhang on 2020-01-18.
//

#ifndef BH_SIM_STOPWATCH_H
#define BH_SIM_STOPWATCH_H

#include <chrono>
#include <stack>
#include <string>
#include <vector>

class Stopwatch final {
    std::string name;
    std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double>> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double>> stop_time;
    int indentLevel;
    std::vector<Stopwatch> finishedChildren;

    // To ensure that only the most recently started stopwatch can be stopped.
    static std::stack<std::string> namesInProgress;

    static std::stack<Stopwatch *> stopwatchesInProgress;

    static bool doOutput;

  public:
    /// Create a new Stopwatch and call start on it right before returning.
    static Stopwatch createAndStart(const std::string &name);

    /// Stop this Stopwatch and output its time elapsed if doOutput is set to true.
    void stopAndOutput();

    /// Sets the doOutput global flag (applies to all Stopwatch instances).
    /// If true, then stopAndOutput will print elapsed time. Otherwise, stopAndOutput will print nothing.
    void static setDoOutput(bool doOutput);

  private:
    explicit Stopwatch(std::string name);
    void start();
    void stop();
    void output() const;
    void outputAsChild(int parentDurationInMS) const;
};

#endif // BH_SIM_STOPWATCH_H
