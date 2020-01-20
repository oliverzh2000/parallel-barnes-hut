#include <utility>


//
// Created by Oliver Zhang on 2020-01-18.
//

#include <chrono>
#include <iostream>

#include "stopwatch.h"

bool Stopwatch::doOutput = true;

Stopwatch Stopwatch::createAndStart(std::string name) {
    Stopwatch stopwatch{std::move(name)};
    stopwatch.start();
    return stopwatch;
}

Stopwatch::Stopwatch(std::string name)
        : name{std::move(name)} {}

void Stopwatch::setDoOutput(bool doOutput) {
    Stopwatch::doOutput = doOutput;
}

void Stopwatch::start() {
    start_time = std::chrono::high_resolution_clock::now();
}

void Stopwatch::stop() {
    stop_time = std::chrono::high_resolution_clock::now();
}

void Stopwatch::output() const {
    if (doOutput) {
        std::chrono::duration duration =  std::chrono::duration_cast<std::chrono::duration<double>>(stop_time - start_time);
        std::cout.width(6);
        std::cout << std::right << int(std::chrono::duration<double, std::milli>(duration).count()) << "ms: " << name << std::endl;
    }
}

void Stopwatch::stopAndOutput() {
    stop();
    output();
}
