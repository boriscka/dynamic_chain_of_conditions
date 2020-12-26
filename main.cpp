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
    Organization o;
    std::cout << o << std::endl;
    //std::cout << o.getByNamePath<long long>("egrul.uk");
    o = j.get<Organization>();
    std::cout << o << std::endl;
    std::cout << j << std::endl;
    return 0;
}
