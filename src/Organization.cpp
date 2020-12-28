//
// Created by boriscka on 26.12.2020.
//

#include "organization.h"
#include <ostream>

void ns::to_json(json& j, const ns::Organization& o) {
    throw std::runtime_error("to_json(json& j, const Organization& o) isn't realized");
}

void ns::from_json(const json& j, ns::Organization& o) {
    auto jEgrul = j.at("egrul");
    jEgrul.at("inn").get_to(o.egrul.inn);
    jEgrul.at("uk").get_to(o.egrul.uk);
    jEgrul.at("ogrn").get_to(o.egrul.ogrn);
    jEgrul.at("opf").get_to(o.egrul.opf);
    jEgrul.at("nalog").get_to(o.egrul.nalog);

    auto jCbr = j.at("cbr");
    auto jEmission = jCbr.at("emission");
    o.cbr.emission = false; // just to allocate memory
    jEmission.get_to(o.cbr.emission.value());
}

std::ostream& ns::operator << (std::ostream &out, const ns::Organization &o) {
    out << "{\n\tegrul: {\n\t\tinn " << o.egrul.inn << ", uk " << o.egrul.uk << ", ogrn " << o.egrul.ogrn
        << ", opf " << o.egrul.opf << ", nalog \"" << o.egrul.nalog << "\"\n\t},\n\tcbr: {\n\t\temission "
        << (o.cbr.emission.has_value() ? (o.cbr.emission.value() ? "true": "false") : "not initialized")
        << "\n\t}\n}" << std::endl;
    return out;
}

void ns::Organization::addToUK(long long amount) {
    egrul.uk += amount;
}

void ns::Organization::changeTax(const std::string &newTaxSystem) {
    egrul.nalog = newTaxSystem;
}
