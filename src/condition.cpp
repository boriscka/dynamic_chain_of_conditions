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
    std::cout << "Rule (" << name << ") is applying..." << std::endl;
    if (ifExecutor(org)) {
        return thenAction;
    } else {
        return elseAction;
    }
}

std::optional<ns::Action::Ptr> ns::Action::applyTo(ns::Organization &org) {
    std::cout << "Result is applying..." << std::endl;
    std::cout << "Result status: " << status << std::endl;
    std::cout << "Result message: " << message << std::endl;
    action(org);
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
        auto rule = std::make_shared<Rule>(jsonRule.key());
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
    auto jsonIf = j.at("if");
    ns::defineIfConditionExecutors(jsonIf, rule.ifExecutor);

    auto jsonThen = j.at("then");
    auto jsonThenType = jsonThen.at("type").get<std::string>();
    if (jsonThenType == "rule") {
        auto relativeRuleName = jsonThen.at("name").get<std::string>();
        rule.relativeRules.insert({ns::SCOPE_THEN_NAME, relativeRuleName});
    } else if (jsonThenType == "result") {
        auto result = std::make_shared<Action>();
        jsonThen.get_to(*result);
        rule.thenAction = result;
    }

    auto jsonElse = j.at("else");
    auto jsonElseType = jsonElse.at("type").get<std::string>();
    if (jsonElseType == "rule") {
        auto relativeRuleName = jsonElse.at("name").get<std::string>();
        rule.relativeRules.insert({ns::SCOPE_ELSE_NAME, relativeRuleName});
    } else if (jsonElseType == "result") {
        auto result = std::make_shared<Action>();
        jsonElse.get_to(*result);
        rule.elseAction = result;
    }
}

void ns::from_json(const json &j, Action &result) {
    j.at("value").get_to(result.status);
    if (result.status == Action::RESULT_STATUS_UNDEFINED) {
        throw std::runtime_error("wrong json result value");
    }
    result.message = j.at("message").get<std::string>();
    if (j.contains("action") && result.status == Action::RESULT_STATUS_ERROR) {
        auto actionName = j.at("action").get<std::string>();
        // hard coded cuz it's out of test task
        if (actionName == "Увеличить") {
            auto debugAmount = 10'000'000ll;
            result.action = [debugAmount] (ns::Organization &org) {
                std::cout << "Action 'Увеличить' is applying..." << std::endl;
                org.addToUK(debugAmount);
            };
        } else if (actionName == "change_tax") {
            auto debugTaxSystem = "УСН";
            result.action = [debugTaxSystem] (ns::Organization &org) {
                std::cout << "Action 'change_tax' is applying..." << std::endl;
                org.changeTax(debugTaxSystem);
            };
        }
    }
}
