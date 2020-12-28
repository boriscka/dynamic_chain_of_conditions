//
// Created by boriscka on 26.12.2020.
//

#ifndef CONDITIONS_ORGANIZATION_H
#define CONDITIONS_ORGANIZATION_H

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>

using namespace nlohmann;

namespace ns {

    class Organization {

    private:
        struct Egrul {
            long long inn;
            long long uk;
            long long ogrn;
            long long opf;
            std::string nalog;
        };

        struct Cbr {
            std::optional<bool> emission;
        };

    private:
        Egrul egrul;
        Cbr cbr;

        friend void to_json(json& j, const Organization& o);
        friend void from_json(const json& j, Organization& o);
        friend std::ostream& operator << (std::ostream &out, const Organization &o);

    public:
        Organization() : egrul{-1, -1, -1, -1} {}

        template<class T>
        const T& getByNamePath(const std::string &namePath) const {
            std::vector<std::string> names;
            size_t lastIndex = 0;

            for (auto delim = namePath.find('.'); delim != std::string::npos; delim = namePath.find('.', lastIndex)) {
                names.push_back(namePath.substr(lastIndex, delim - lastIndex));
                lastIndex = delim + 1;
            }
            names.push_back(namePath.substr(lastIndex, namePath.size() - lastIndex));

            if (!names.empty()) {
                auto &rootName = names[0];
                if (rootName == "egrul" && names.size() > 1) {
                    auto &egrulMember = names[1];
                    if (egrulMember == "inn") {
                        return returnSameTypeOnly<T, decltype(egrul.inn)>(egrul.inn);
                    } else if (egrulMember == "uk") {
                        return returnSameTypeOnly<T, decltype(egrul.uk)>(egrul.uk);
                    } else if (egrulMember == "ogrn") {
                        return returnSameTypeOnly<T, decltype(egrul.ogrn)>(egrul.ogrn);
                    } else if (egrulMember == "opf") {
                        return returnSameTypeOnly<T, decltype(egrul.opf)>(egrul.opf);
                    } else if (egrulMember == "nalog") {
                        return returnSameTypeOnly<T, decltype(egrul.nalog)>(egrul.nalog);
                    }
                } else if (rootName == "cbr" && names.size() > 1) {
                    auto &cbrMember = names[1];
                    if (cbrMember == "emission" && cbr.emission.has_value()) {
                        auto& cbrEmission = cbr.emission.value();
                        return returnSameTypeOnly<T, decltype(cbrEmission)>(cbrEmission);
                    }
                }
            }

            throw std::runtime_error("Something went wrong. Maybe there is no that field: " + namePath);
        }

        template <class TargetType, class ValueType>
        const typename std::enable_if<std::is_same_v<TargetType, ValueType>, TargetType>::type &
        returnSameTypeOnly(const ValueType &value) const {
            return value;
        }

        template <class TargetType, class ValueType>
        const typename std::enable_if<not std::is_same_v<TargetType, ValueType>, TargetType>::type &
        returnSameTypeOnly(const ValueType &value) const {
            if (typeid(TargetType).hash_code() == typeid(ValueType).hash_code()) { // get rid (it's kinda debug)
                return TargetType();
            }
            throw std::runtime_error("Expected type and current type ain't same: (target type) "
                                     + std::to_string(typeid(TargetType).hash_code()) + ", (actual type) "
                                     + std::to_string(typeid(ValueType).hash_code()));
        }

    };


    void to_json(json& j, const Organization& o);
    void from_json(const json& j, Organization& o);

    std::ostream& operator << (std::ostream &out, const ns::Organization &o);
}



#endif //CONDITIONS_ORGANIZATION_H
