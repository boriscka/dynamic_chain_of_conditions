//
// Created by boriscka on 26.12.2020.
//

#ifndef CONDITIONS_CONDITION_H
#define CONDITIONS_CONDITION_H

#include <organization.h>
#include <string>
#include <utility>
#include <functional>
#include <map>

namespace ns {

    class Result {

    public:
        enum ResultType {
            RESULT_ACTION,
            RESULT_RULE,
            RESULT_UNDEFINED = -1
        };

        enum ResultStatus {
            STATUS_SUCCESS,
            STATUS_WARNING,
            STATUS_ERROR,
            STATUS_UNDEFINED = -1
        };

    public:
        virtual void applyTo(Organization &org);
    };


    class Rule: protected Result {

        std::string name;
        std::function<bool(const Organization &)> ifExecutor;
        std::function<void(Organization &)> thenExecutor;
        std::function<void(Organization &)> elseExecutor;

    public:

        Rule() = default;
        explicit Rule(std::string name) : name(std::move(name)) {}

        void applyTo(Organization &org) override;
    };

    class Condition {

        Rule rootRule;
        std::map<std::string, Rule> rules;
        bool isInitialized;

    public:

        Condition() : isInitialized(false) {}

        void applyTo(Organization &org);
    };

}

#endif //CONDITIONS_CONDITION_H
