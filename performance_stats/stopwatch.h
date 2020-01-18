//
// Created by Oliver Zhang on 2020-01-18.
//

#ifndef BH_SIM_STOPWATCH_H
#define BH_SIM_STOPWATCH_H

#include <string>

class Stopwatch final {
    std::string name;
    std::chrono::time_point<std::chrono::high_resolution_clock , std::chrono::duration<double>> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock , std::chrono::duration<double>> stop_time;

public:
    static Stopwatch createAndStart(std::string name);
    void stopAndOutput();

private:

    explicit Stopwatch(std::string name);
    void start();
    void stop();
    void output() const;
};


#endif //BH_SIM_STOPWATCH_H
