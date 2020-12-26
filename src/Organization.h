//
// Created by boriscka on 26.12.2020.
//

#ifndef CONDITIONS_ORGANIZATION_H
#define CONDITIONS_ORGANIZATION_H

#include <ostream>
#include <string>
#include <nlohmann/json.hpp>
#include <stdexcept>

using namespace nlohmann;

namespace ns {

    class Organization {

        struct Egrul {
            long inn;
            long long uk;
            long long ogrn;
            int opf;
            std::string nalog;
        };

        struct Cbr {
            std::optional<bool> emission;
        };

        Egrul egrul;
        Cbr cbr;

        friend void to_json(json& j, const Organization& o);
        friend void from_json(const json& j, Organization& o);
        friend std::ostream& operator << (std::ostream &out, const Organization &o);

    public:
        Organization() : egrul{-1, -1, -1, -1} {}

        template<typename T>
        const T& getByNamePath(const std::string &namePath);
    };


    void to_json(json& j, const Organization& o);
    void from_json(const json& j, Organization& o);

    std::ostream& operator << (std::ostream &out, const ns::Organization &o);
}



#endif //CONDITIONS_ORGANIZATION_H
