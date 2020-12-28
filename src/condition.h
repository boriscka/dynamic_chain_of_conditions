//
// Created by boriscka on 26.12.2020.
//

#ifndef CONDITIONS_CONDITION_H
#define CONDITIONS_CONDITION_H

#include <organization.h>
#include <condition_functions.h>

#include <string>
#include <utility>
#include <functional>
#include <optional>
#include <map>
#include <stdexcept>
#include <memory>


namespace ns {

    class Rule;
    using RuleMap = std::map<std::string, std::shared_ptr<Rule>>;

    static const char* SCOPE_THEN_NAME = "then";
    static const char* SCOPE_ELSE_NAME = "else";


    class Action {

        friend void to_json(json& j, const Action&);
        friend void from_json(const json& j, Action&);

    public:
        using Ptr = std::shared_ptr<Action>;

        enum ResultStatus {
            RESULT_STATUS_SUCCESS = 1,
            RESULT_STATUS_WARNING,
            RESULT_STATUS_ERROR,
            RESULT_STATUS_UNDEFINED = -1
        };

    private:
        std::string message;
        ResultStatus status{RESULT_STATUS_UNDEFINED};
        ResultAction action = [](Organization &o) { };


    public:
        virtual ~Action() = default;

        /**
         * It applies a rule to an organization
         * @param org
         * @return next Action (to avoiding a recursion)
         */
        virtual std::optional<Action::Ptr> applyTo(Organization &org);
    };


    class Rule: public Action {

    private:

        std::string name;
        std::map<std::string, std::string> relativeRules;

        ConditionExecutor ifExecutor = [](const Organization &o){ return true;};
        Action::Ptr thenAction = nullptr;
        Action::Ptr elseAction = nullptr;

        friend void to_json(json& j, const Rule&);
        friend void from_json(const json& j, Rule&);

    public:

        Rule() = default;
        explicit Rule(std::string name) : name(std::move(name)) {}

        std::optional<Action::Ptr> applyTo(Organization &org) override;

        void linkToRules(const RuleMap & rules);
    };

    class Condition {

        std::shared_ptr<Rule> rootRule;
        bool isInitialized;

        friend void to_json(json& j, const Condition&);
        friend void from_json(const json& j, Condition&);

    public:

        Condition() : isInitialized(false) {}

        void applyTo(Organization &org);
    };


    void to_json(json& j, const Condition& cond);
    void from_json(const json& j, Condition& cond);
    void to_json(json& j, const Rule& rule);
    void from_json(const json& j, Rule& rule);
    void to_json(json& j, const Action& result);
    void from_json(const json& j, Action& result);
}

#endif //CONDITIONS_CONDITION_H
