#include <iostream>

#include <nlohmann/json.hpp>

#include <organization.h>

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
    //o.getByNamePath<int>("jjj");
    auto organization1 = j.get<Organization>();
    std::cout << organization1 << std::endl;
    std::cout << j << std::endl;
    return 0;
}
