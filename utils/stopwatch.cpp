//
// Created by Oliver Zhang on 2020-01-18.
//

#include "stopwatch.h"

#include <chrono>
#include <iostream>
#include <stack>
#include <string>
#include <utility>

bool Stopwatch::doOutput = true;
std::stack<std::string> Stopwatch::namesInProgress;

Stopwatch Stopwatch::createAndStart(const std::string &name) {
    Stopwatch stopwatch{name};
    stopwatch.start();
    return stopwatch;
}

Stopwatch::Stopwatch(std::string name) : name{std::move(name)} {}

void Stopwatch::setDoOutput(bool doOutput) {
    Stopwatch::doOutput = doOutput;
}

void Stopwatch::start() {
    namesInProgress.emplace(name);
    start_time = std::chrono::high_resolution_clock::now();
}

void Stopwatch::stop() {
    if (name != namesInProgress.top()) {
        throw std::runtime_error("only the most recently started Stopwatch can be stopped");
    }
    stop_time = std::chrono::high_resolution_clock::now();
    namesInProgress.pop();
}

void Stopwatch::output() const {
    if (doOutput) {
        std::chrono::duration duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop_time - start_time);
        std::cout.width(7);
        int indentLevel = namesInProgress.size();
        std::cout << std::right << int(std::chrono::duration<double, std::milli>(duration).count())
                  << "ms: " << std::string(" ", 2 * indentLevel) << name << std::endl;
    }
}

void Stopwatch::stopAndOutput() {
    stop();
    output();
}
