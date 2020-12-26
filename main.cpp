#include <iostream>

#include <nlohmann/json.hpp>

#include <organization.h>
#include <condition.h>

using namespace  ns;

int main() {
    auto j = R"({
                  "egrul":
                  {
                     "inn": 123456789,
                     "uk": 10000000,
                     "ogrn": 1117847400120,
                     "opf": 12211,
                     "nalog": "ОСНО"
                  },
                  "cbr":
                  {
                     "some_value": -1,
                     "emission": false
                  }
                })"_json;
    std::cout << j << std::endl;

    Organization o;
    std::cout << o << std::endl;
    try {
        o = j.get<Organization>();
    } catch(std::exception &e) {
        std::cout << "Organization error: " << e.what() << std::endl;
        return 1;
    }
    std::cout << o << std::endl;

    Condition cond;
    try {
        cond = j.get<Condition>();
        cond.applyTo(o);
    } catch(std::exception &e) {
        std::cout << "Condition error: " << e.what() << std::endl;
        return 2;
    }
    std::cout << o << std::endl;
    return 0;
}
