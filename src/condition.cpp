//
// Created by boriscka on 26.12.2020.
//

#include <iostream>
#include <string>
#include <condition.h>
#include <condition_functions.h>


void ns::Condition::applyTo(ns::Organization &org) {
    if (!isInitialized) {
        throw std::runtime_error("ns::Condition::applyTo: not initialized");
    }
    if (rootRule == nullptr) {
        std::cout << "There is no a rule in a current condition!" << std::endl;
        return;
    }
    for (auto nextAction = rootRule->applyTo(org); nextAction.has_value() && nextAction.value() != nullptr;) {
        nextAction = nextAction.value()->applyTo(org);
    }
}

std::optional<ns::Action::Ptr> ns::Rule::applyTo(ns::Organization &org) {
    if (ifExecutor(org)) {
        return thenAction;
    } else {
        return elseAction;
    }
}

std::optional<ns::Action::Ptr> ns::Action::applyTo(ns::Organization &org) {
    std::cout << "ns::Result::applyTo: success" << std::endl;
    return nullptr;
}

void ns::Rule::linkToRules(const ns::RuleMap & rules) {
    for (auto &relativeData : relativeRules) {
        auto ifScopeName = relativeData.first;
        auto relativeRuleName = relativeData.second;
        auto relativeRuleIter = rules.find(relativeRuleName);
        if (relativeRuleIter == rules.end()) {
            throw std::runtime_error("that rule name doesn't exist in rules map");
        }

        if (ifScopeName == ns::SCOPE_THEN_NAME) {
            thenAction = relativeRuleIter->second;
        }
        else if (ifScopeName == ns::SCOPE_ELSE_NAME) {
            elseAction = relativeRuleIter->second;
        }
    }
    relativeRules.clear();
}

// JSON
void ns::to_json(json &j, const ns::Condition &cond) {
    throw std::runtime_error("ns::to_json(json &j, const Condition &): not implemented");
}
void ns::to_json(json &j, const ns::Rule &rule) {
    throw std::runtime_error("ns::to_json(json &j, const Rule &): not implemented");
}
void ns::to_json(json &j, const ns::Action &result) {
    throw std::runtime_error("ns::to_json(json &j, const Result &): not implemented");
}

void ns::from_json(const json &j, ns::Condition &cond) {
    ns::RuleMap rules;
    for (auto jsonRule = j.begin(); jsonRule != j.end(); jsonRule++) {
        auto rule = std::make_shared<Rule>();
        jsonRule.value().get_to(*rule);
        rules.insert({jsonRule.key(), rule});
    }
    for (auto &rule : rules) {
        rule.second->linkToRules(rules);
    }
    if (!rules.empty()) {
        cond.rootRule = rules.begin()->second;
    }
    cond.isInitialized = true;
}

void ns::from_json(const json &j, ns::Rule &rule) {
    auto jsonThen = j.at("then");
    if (jsonThen.at("type") == "rule") {
        auto relativeRuleName = jsonThen.at("name").get<std::string>();
        rule.relativeRules.insert({ns::SCOPE_THEN_NAME, relativeRuleName});
    }

    auto jsonElse = j.at("else");
    if (jsonElse.at("type") == "rule") {
        auto relativeRuleName = jsonElse.at("name").get<std::string>();
        rule.relativeRules.insert({ns::SCOPE_ELSE_NAME, relativeRuleName});
    }

    auto jsonIf = j.at("if");
    ns::defineIfConditionExecutors(jsonIf, rule.ifExecutor);
}

void ns::from_json(const json &j, ns::Action &result) {

}
