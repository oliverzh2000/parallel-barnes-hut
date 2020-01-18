#include <utility>

#include <utility>

//
// Created by Oliver Zhang on 2020-01-18.
//

#include <chrono>
#include <iostream>

#include "stopwatch.h"

Stopwatch Stopwatch::createAndStart(std::string name) {
    Stopwatch stopwatch{std::move(name)};
    stopwatch.start();
    return stopwatch;
}

Stopwatch::Stopwatch(std::string name)
        : name{std::move(name)} {}

void Stopwatch::start() {
    start_time = std::chrono::high_resolution_clock::now();
}

void Stopwatch::stop() {
    stop_time = std::chrono::high_resolution_clock::now();
}

void Stopwatch::output() const {
    std::chrono::duration duration =  std::chrono::duration_cast<std::chrono::duration<double>>(stop_time - start_time);
//    std::cout << name << ": " << duration.count() << "s" << std::endl;
    std::cout << name << ": " << int(std::chrono::duration<double, std::milli>(duration).count()) << "ms" << std::endl;
}

void Stopwatch::stopAndOutput() {
    stop();
    output();
}
