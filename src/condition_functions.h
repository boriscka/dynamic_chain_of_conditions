//
// Created by boriscka on 29.12.2020.
//

#ifndef CONDITIONS_CONDITION_FUNCTIONS_H
#define CONDITIONS_CONDITION_FUNCTIONS_H

#include <organization.h>
#include <nlohmann/json.hpp>

#include <stdexcept>
#include <string>

namespace ns {

    using ConditionExecutor = std::function<bool(const ns::Organization &)>;


    template <class T>
    inline bool isEqual(const T &leftVar, const T &rightVar) {
        return leftVar == rightVar;
    }

    template <class T>
    inline bool isGreater(const T &leftVar, const T &rightVar) {
        return leftVar > rightVar;
    }

    template <typename T>
    inline bool isInRange(const T &var, const T &minVal, const T &maxVal) {
        if (minVal > maxVal) {
            throw std::runtime_error("Error in isInRange(): bottom border (" + std::to_string(minVal)
                                     + ") is greater than top border (" + std::to_string(maxVal) + ").");
        }
        return var == minVal || var > minVal || var < maxVal || var == maxVal;
    }


    template <class T>
    inline void defineEqualFunction(const std::string &field, const T &value, ConditionExecutor &function) {
        function = ConditionExecutor([field, value] (const ns::Organization &org) {
            return isEqual(org.getByNamePath<T>(field), value);
        });
    }

    template <class T>
    inline void defineGreaterThanOrEqualFunction(const std::string &field, const T &value, ConditionExecutor &function) {
        function = ConditionExecutor([field, value] (const ns::Organization &org) {
            const auto &orgValue = org.getByNamePath<T>(field);
            return isEqual(orgValue, value) || isGreater(orgValue, value);
        });
    }

    template <typename T>
    inline void defineRangeFunction(const std::string &field, const T &minValue, const T &maxValue, ConditionExecutor &function) {
        function = ConditionExecutor([field, minValue, maxValue] (const ns::Organization &org) {
            auto orgValue = org.getByNamePath<T>(field);
            return isInRange(orgValue, minValue, maxValue);
        });
    }

// defines if condition executors
    inline void defineIfConditionExecutors(const json &jsonIfEntity, ConditionExecutor &function) {
        auto jsonCond = jsonIfEntity.at("cond");

        if (jsonCond == "equal") {
            auto field = jsonIfEntity.at("field").get<std::string>();
            auto value = jsonIfEntity.at("value");
            if (value.is_string()) {
                auto strVal = value.get<std::string>();
                defineEqualFunction(field, strVal, function);
            } else if (value.is_boolean()) {
                bool boolVal = value.get<bool>();
                defineEqualFunction(field, boolVal, function);
            } else if (value.is_number_integer()) {
                auto longVal = value.get<long long>();
                defineEqualFunction(field, longVal, function);
            } else if (value.is_number_float()) {
                auto doubleVal = value.get<long double>();
                defineEqualFunction(field, doubleVal, function);
            } else {
                throw std::runtime_error("there is wrong type in json object: " + std::string(value.type_name()));
            }
        }

        if (jsonCond == "greater_then_on_equal_to") {
            auto field = jsonIfEntity.at("field").get<std::string>();
            auto value = jsonIfEntity.at("value");
            if (value.is_string()) {
                auto strVal = value.get<std::string>();
                defineGreaterThanOrEqualFunction(field, strVal, function);
            } else if (value.is_number_integer()) {
                auto longVal = value.get<long long>();
                defineGreaterThanOrEqualFunction(field, longVal, function);
            } else if (value.is_number_float()) {
                auto doubleVal = value.get<long double>();
                defineGreaterThanOrEqualFunction(field, doubleVal, function);
            } else {
                throw std::runtime_error("there is wrong type in json object: " + std::string(value.type_name()));
            }
        }

        if (jsonCond == "range") {
            auto field = jsonIfEntity.at("field").get<std::string>();
            auto minValue = jsonIfEntity.at("min");
            auto maxValue = jsonIfEntity.at("max");
            if (minValue.is_number_integer() && maxValue.is_number_integer()) {
                auto minLongVal = minValue.get<long long>();
                auto maxLongVal = maxValue.get<long long>();
                defineRangeFunction(field, minLongVal, maxLongVal, function);
            } else if ((minValue.is_number_float() && maxValue.is_number_float())
                       || ((minValue.is_number_float() || maxValue.is_number_float())
                           && (minValue.is_number_integer() || maxValue.is_number_integer())))
            {
                auto minDoubleVal = minValue.get<long double>();
                auto maxDoubleVal = maxValue.get<long double>();
                defineRangeFunction(field, minDoubleVal, maxDoubleVal, function);
            } else {
                throw std::runtime_error("there is wrong type in json object: min value "
                                         + std::string(minValue.type_name()) + ", max value "
                                         + std::string(maxValue.type_name()));
            }
        }
    }
}

#endif //CONDITIONS_CONDITION_FUNCTIONS_H
