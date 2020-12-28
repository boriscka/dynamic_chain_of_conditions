//
// Created by boriscka on 26.12.2020.
//

#include <iostream>
#include <condition.h>


void ns::Condition::applyTo(ns::Organization &org) {
    if (!isInitialized) {
        throw std::runtime_error("ns::Condition::applyTo: not initialized");
    }
    std::cout << "Condition::applyTo: success" << std::endl;
    auto firstRuleIterator = rules.begin();
    if (firstRuleIterator == rules.end()) {
        std::cout << "There is no a rule in a current condition!" << std::endl;
        return;
    }
    for(auto nextRuleName = firstRuleIterator->second.applyTo(org); nextRuleName.has_value();) {
        auto nextRuleIterator = rules.find(nextRuleName.value());
        if (nextRuleIterator == rules.end()) {
            throw std::runtime_error("There is no rule with name \"" + nextRuleName.value() + "\"");
        }
        nextRuleName = nextRuleIterator->second.applyTo(org);
    }
}

std::optional<std::string> ns::Action::applyTo(Organization &org) {
    std::cout << "ns::Result::applyTo: success" << std::endl;
    return {};
}

std::optional<std::string> ns::Rule::applyTo(Organization &org) {
    std::cout << "Rule::applyTo: success" <<std::endl;
    return {};
}

// JSON
void ns::to_json(json &j, const Condition &cond) {
    throw std::runtime_error("ns::to_json(json &j, const Condition &): not implemented");
}
void ns::to_json(json &j, const Rule &rule) {
    throw std::runtime_error("ns::to_json(json &j, const Rule &): not implemented");
}
void ns::to_json(json &j, const Action &result) {
    throw std::runtime_error("ns::to_json(json &j, const Result &): not implemented");
}

void ns::from_json(const json &j, Condition &cond) {
    for (auto rule = j.begin(); rule != j.end(); rule++) {
        cond.rules.insert({rule.key(), rule.value().get<Rule>()});
    }
    cond.isInitialized = true;
}

void ns::from_json(const json &j, Rule &rule) {

}

void ns::from_json(const json &j, Action &result) {

}
