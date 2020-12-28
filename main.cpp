#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>

#include <organization.h>
#include <condition.h>

using namespace  ns;

int main() {
    std::ifstream fileCondition1("condition1.json");
    std::ifstream fileCondition2("condition2.json");
    std::ifstream fileOrganization1("organization.json");
    if (!fileCondition1.is_open() || !fileCondition2.is_open() || !fileOrganization1.is_open()) {
        std::cout << "Error: Problem during an opening files: condition1.json, condition2.json, organization.json"
                  << std::endl;
        return -1;
    }

    json jsonCondition1, jsonCondition2, jsonOrganization1;
    fileCondition1 >> jsonCondition1;
    fileCondition2 >> jsonCondition2;
    fileOrganization1 >> jsonOrganization1;

    std::cout << jsonCondition1 << std::endl;
    std::cout << jsonCondition2 << std::endl;
    std::cout << jsonOrganization1 << std::endl;

    Organization organization;
    Condition condition_1;
    Condition condition_2;

    try {
        organization = jsonOrganization1.get<Organization>();
        condition_1 = jsonCondition1.get<Condition>();
        condition_2 = jsonCondition2.get<Condition>();
    } catch(std::exception &e) {
        std::cout << "Error during a deserialization from json objects: " << e.what() << std::endl;
        return -2;
    }

    try {
        std::cout << std::endl << "Organization before an execution of conditions: \n" << organization << std::endl;

        condition_1.applyTo(organization);
        std::cout << std::endl << "Organization after first conditions: \n" << organization << std::endl;

        condition_2.applyTo(organization);
        std::cout << std::endl << "Organization after second conditions: \n" << organization << std::endl;

    } catch(std::exception &e) {
        std::cout << "Error during an execution of conditions: " << e.what() << std::endl;
        return -3;
    }

    return 0;
}
