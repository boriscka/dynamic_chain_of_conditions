#include <iostream>

#include <nlohmann/json.hpp>

int main() {
    auto j = R"({"gender":"male", "isTrue": true})"_json;
    j["gender"] = "female";
    std::cout << j << std::endl;
    return 0;
}
