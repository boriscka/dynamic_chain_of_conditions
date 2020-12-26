//
// Created by boriscka on 26.12.2020.
//

#include <iostream>
#include <condition.h>


void ns::Condition::applyTo(Organization &org) {
    if (!isInitialized) {
        throw std::runtime_error("ns::Condition::applyTo: not initialized");
    }
    std::cout << "Condition::applyTo: success" << std::endl;
    std::cout << org << std::endl;
}

void ns::Result::applyTo(Organization &org) {
    std::cout << "ns::Result::applyTo: success" << std::endl;
}

void ns::Rule::applyTo(Organization &org) {
    std::cout << "Rule::applyTo: success" <<std::endl;
}

// JSON
void ns::to_json(json &j, const Condition &cond) {
    throw std::runtime_error("ns::to_json(json &j, const Condition &): not implemented");
}
void ns::to_json(json &j, const Rule &rule) {
    throw std::runtime_error("ns::to_json(json &j, const Rule &): not implemented");
}
void ns::to_json(json &j, const Result &result) {
    throw std::runtime_error("ns::to_json(json &j, const Result &): not implemented");
}

void ns::from_json(const json &j, Condition &cond) {

    cond.isInitialized = true;
}

void ns::from_json(const json &j, Rule &rule) {

}

void ns::from_json(const json &j, Result &result) {

}
