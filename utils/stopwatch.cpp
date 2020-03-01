//
// Created by Oliver Zhang on 2020-01-18.
//

#include "stopwatch.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <stack>
#include <string>
#include <utility>

bool Stopwatch::doOutput = true;
std::stack<std::string> Stopwatch::namesInProgress;
std::stack<Stopwatch *> Stopwatch::stopwatchesInProgress;

Stopwatch Stopwatch::createAndStart(const std::string &name) {
    Stopwatch stopwatch{name};
    stopwatch.start();
    return stopwatch;
}

Stopwatch::Stopwatch(std::string name) : name{std::move(name)}, indentLevel{static_cast<int>(namesInProgress.size())} {}

void Stopwatch::setDoOutput(bool doOutput) {
    Stopwatch::doOutput = doOutput;
}

void Stopwatch::start() {
    namesInProgress.emplace(name);
    stopwatchesInProgress.emplace(this);
    start_time = std::chrono::high_resolution_clock::now();
}

void Stopwatch::stop() {
    if (name != namesInProgress.top()) {
        throw std::runtime_error("only the most recently started Stopwatch can be stopped: " + name);
    }
    stop_time = std::chrono::high_resolution_clock::now();
    namesInProgress.pop();
    stopwatchesInProgress.pop(); // Remove this stopwatch.
    if (!stopwatchesInProgress.empty()) { // Only do this if this stopwatch has a parent.
        stopwatchesInProgress.top()->finishedChildren.emplace_back(*this);
    }
}

void Stopwatch::output() const {
    std::chrono::duration duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop_time - start_time);
    int durationInMS = int(std::chrono::duration<double, std::milli>(duration).count());
    outputAsChild(durationInMS);
}

void Stopwatch::outputAsChild(int parentDurationInMS) const {
    std::chrono::duration duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop_time - start_time);
    std::cout.width(7);
    int durationInMS = int(std::chrono::duration<double, std::milli>(duration).count());
    std::cout << std::right << durationInMS << "ms ";
    std::cout << std::string(2 * indentLevel, ' ');
    std::cout.width(3);
    std::cout << std::right << std::to_string(int(durationInMS / double(parentDurationInMS == 0 ? 1 : parentDurationInMS) * 100)) + "%: "
            << name << std::endl;
    for (const Stopwatch &finishedChild : finishedChildren) {
        finishedChild.outputAsChild(durationInMS);
    }
}

void Stopwatch::stopAndOutput() {
    stop();
    if (doOutput && stopwatchesInProgress.empty()) {
        output();
    }
}
