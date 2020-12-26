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
#include <stdexcept>

namespace ns {

    class Result {

        friend void to_json(json& j, const Result&);
        friend void from_json(const json& j, Result&);

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
        virtual ~Result() = default;
        virtual void applyTo(Organization &org);
    };


    class Rule: public Result {

        std::string name;
        std::function<bool(const Organization &)> ifExecutor;
        std::function<void(Organization &)> thenExecutor;
        std::function<void(Organization &)> elseExecutor;

        friend void to_json(json& j, const Rule&);
        friend void from_json(const json& j, Rule&);

    public:

        Rule() = default;
        explicit Rule(std::string name) : name(std::move(name)) {}

        void applyTo(Organization &org) override;
    };

    class Condition {

        Rule * rootRule;
        std::map<std::string, Rule> rules;
        bool isInitialized;

        friend void to_json(json& j, const Condition&);
        friend void from_json(const json& j, Condition&);

    public:

        Condition() : isInitialized(false), rootRule(nullptr) {}

        void applyTo(Organization &org);
    };


    void to_json(json& j, const Condition& cond);
    void from_json(const json& j, Condition& cond);
    void to_json(json& j, const Rule& rule);
    void from_json(const json& j, Rule& rule);
    void to_json(json& j, const Result& result);
    void from_json(const json& j, Result& result);
}

#endif //CONDITIONS_CONDITION_H
