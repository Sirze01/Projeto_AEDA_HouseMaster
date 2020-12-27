#include "HouseMaster.h"

/**
 * @brief getter
 * @return the affiliates
 */
BST<HouseMasterAffiliate > &HouseMaster::getAffiliates() {
    return _affiliates;
}

HouseMaster::HouseMaster() : _affiliates(HouseMasterAffiliate()) {

}

void HouseMaster::registerAffiliate(const HouseMasterAffiliate& affiliate) {
    _affiliates.insert(affiliate);
    for (const auto &client : affiliate.getClients()) {
        _clientContacts.insert(client.second);
    }
}

clientHT HouseMaster::getContacts() const {
    return _clientContacts;
}

void HouseMaster::registerClient(Client *client) {
    _clientContacts.insert(client);
}

HouseMaster::HouseMaster(std::ifstream affiliates) : _affiliates(HouseMasterAffiliate()) {
    for (std::string line; std::getline(affiliates, line);) {
        std::stringstream lss(line);
        // name
        std::string name{};
        std::getline(lss, name, ',');
        // responsible
        std::string responsible{};
        std::getline(lss, responsible, ',');
        // location
        std::string location{};
        std::getline(lss, location, ',');
        auto h1 = HouseMasterAffiliate(std::ifstream("../data/usernames.txt"),
                                           std::ifstream("../data/collabs.txt"),
                                           std::ifstream("../data/clients.txt"),
                                           std::ifstream("../data/services.txt"),
                                           std::ifstream("../data/finances.txt"),
                                           std::ifstream("../data/history.txt"),
                                            location, responsible, name);
        registerAffiliate(h1);
    }
}

void HouseMaster::writeAffiliatesInfo() {
    std::ofstream affiliatesFile("../data/affiliate.txt");
    if (affiliatesFile.is_open()) {
        BSTItrIn<HouseMasterAffiliate> current(_affiliates);
        for (; !current.isAtEnd(); current.advance()) {
            affiliatesFile << current.retrieve().getAffiliateName()
            << ',' << current.retrieve().getLocation() << ',' << current.retrieve().getAdmin().getName() << '\n';
        }
            affiliatesFile.close();
    } else throw HouseMasterAffiliate::UnableToWriteFile("Unable to write in affiliates file");  // TODO change?
}



vector<HouseMasterAffiliate> HouseMaster::getAffiliatesByLocation(string location)
{
    vector<HouseMasterAffiliate> affiliates_from_location;
    for(auto it = _affiliates.begin(); it != _affiliates.end(); it++)
    {
        if((*it).getLocation() == location)
        {
            affiliates_from_location.push_back(*it);
        }
    }
    return affiliates_from_location;
}



vector<HouseMasterAffiliate> HouseMaster::getAffiliatesByResponsible(string responsible)
{
    vector<HouseMasterAffiliate> affiliates_from_responsible;
    for(auto it = _affiliates.begin(); it != _affiliates.end(); it++)
    {
        if((*it).getAdmin().getName() == responsible)
        {
            affiliates_from_responsible.push_back(*it);
        }
    }
    return affiliates_from_responsible;
}


float HouseMaster::getTotalFinances()
{
    float total_finances;
    for(auto it = _affiliates.begin(); it != _affiliates.end(); it++)
    {
        total_finances += (*it).getEarnings();
    }
    return total_finances;
}