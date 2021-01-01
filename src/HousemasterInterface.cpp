#include "HousemasterInterface.h"


HousemasterInterface::HousemasterInterface(const HouseMaster &housemaster) : _currentAffiliate(), _houseMaster(housemaster) {

}


void HousemasterInterface::showAffiliateInterface(HouseMasterAffiliate &affiliate) {
    bool running = true;
    Interface affiliateInterface(affiliate);
    /*
    while (running) {
        affiliateInterface.selectRole(running);
    }*/
    _houseMaster.removeAffiliate(affiliate);
    _houseMaster.registerAffiliate(affiliateInterface.getHouseMasterState());
}


HouseMasterAffiliate HousemasterInterface::selectAffiliate(bool &running) {
    std::map<std::string, std::function<void()> > options{};
    auto affiliates = _houseMaster.getAffiliates();
    HouseMasterAffiliate selection{};
    BSTItrIn<HouseMasterAffiliate> current(affiliates);
    std::set<HouseMasterAffiliate> hms{};
    for (; !current.isAtEnd(); current.advance()) {
        hms.insert(current.retrieve());
    }
    for (const auto & i : hms) {
        options.insert(std::pair<std::string, std::function<void()>>(i.getAffiliateName(),[&](){
            selection = i;
        }));
    }
    Menu affiliateSelection("Select your affiliate", options);
    affiliateSelection.show();
    affiliateSelection.select();
    affiliateSelection.execute(running);
    return selection;
}


void HousemasterInterface::runAffiliateInterface(bool &running) {
    while (running) {
        HouseMasterAffiliate affiliate = selectAffiliate(running);
        std::cout << "Showing " << affiliate.getAffiliateName() << "\n";
        showAffiliateInterface(affiliate);
    }
}


void HousemasterInterface::firstInterface(bool &running) {
    bool innerRunning = true;
    Menu start("Welcome to Housemaster", {{"HouseMaster Administration", [&](){
        std::cout << "Welcome to HouseMaster. You have ADMIN privilege.\n";
        adminLogin();
        showInterface(running);
        while (innerRunning) {
            start.show();
            start.select();
            start.execute(innerRunning);
            std::cin.ignore();
        }
    }},{"Login Client", [&](){
        clientLogin();
        bool innerRunning = true;
        Interface clientInterface(_currentAffiliate, _user, client);
        while (innerRunning) {
            clientInterface.clientOperations(innerRunning);
        }
    }}, {"Login Collaborator", [&](){
        collabLogin();
        bool innerRunning = true;
        Interface collabInterface(_currentAffiliate, _user, collaborator);
        std::cout << "Login succeeded for " << _user->getName() << "\n";
        while (innerRunning) {
            collabInterface.collaboratorOperations(innerRunning);
        }
    }}, { "Login Responsible", [&](){
        std::cout << "Welcome to HouseMaster. You have RESPONSIBLE privilege.\n";
        bool innerRunning2 = true;
        HouseMasterAffiliate affiliate = selectAffiliate(innerRunning2);
        responsibleLogin(affiliate);
        //newInterface.responsibleOperations(innerRunning);
        while (innerRunning) {
            start.show();
            start.select();
            start.execute(innerRunning);
            std::cin.ignore();
        }
      }}});
    start.show();
    start.select();
    start.execute(running);
}



/**
 * @brief admin login
 */
void HousemasterInterface::adminLogin() {
    std::string password{};
    std::cout << "Password : ";
    std::cin >> password;
    for (int i = 0; i <= 1; i++) {
        if (password == "admin") {
            std::cout << "success \n";
            return;
        } else {
            std::cout << "Wrong password. Try again:\n";
            std::cin >> password;
        }
    }
    std::cout << "Too many tries! No admin for you. Logging out...\n";
    exit(0);
}


/**
 * @brief responsible login
 */
void HousemasterInterface::responsibleLogin(const HouseMasterAffiliate &hma) {
    std::string password{};
    std::cout << "Password : ";
    std::cin >> password;
    for (int i = 0; i <= 1; i++) {
        if (password == hma.getAdmin().getPassword()) {
            std::cout << "success \n";
            return;
        } else {
            std::cout << "Wrong password. Try again:\n";
            std::cin >> password;
        }
    }
    std::cout << "Too many tries! Logging out...\n";
    exit(0);
}




/**
 * @brief collab login
 */
void HousemasterInterface::collabLogin() {
    std::string username{};
    std::cout << "Username: ";
    std::cin.ignore();
    std::getline(std::cin, username);
    bool done = false;
    while (!done) {
        try{
            done = true;
            _user = _houseMaster.findByUsername(username);
            _houseMasterAffiliate = _houseMaster.findAffiliateByCollab(dynamic_cast<Collaborator*>(_user));
        }
        catch (const HouseMaster::NonexistentUsername &e) {
            done = false;
            std::cout << e.what() << " Please try again\n";
            std::cout << "Username : ";
            std::cin >> username;
        }
    }
}

/**
 * @brief client login
 */

void HousemasterInterface::clientLogin() {
    std::string email{};
    std::cout << "E-mail: ";
    std::cin.ignore();
    std::getline(std::cin, email);
    bool done = false;
    while (!done) {
        try {
            done = true;
            Client* client = _houseMaster.findClientByEmail(email);
            _currentAffiliate = _houseMaster.findAffiliateByClient(client);
            _user = client;
            std::cout << "Logged " << client->getName() << " to " << _currentAffiliate.getAffiliateName() << '\n';
        }
        catch (const HouseMaster::NonexistentClient &e) {
            done = false;
            std::cout << e.what() << " Please try again\n";
            std::cout << "E-mail: ";
            std::cin >> email;
        }
    }
}



void HousemasterInterface::showInterface(bool &running) {
    Menu start("Welcome to Housemaster", {{"Show Finances", [&](){
        showTotalFinances(_houseMaster);
        std::cin.ignore();
    }},{"Filter by location", [&](){
        bool innerRunning = true;
        std::string location = selectLocation(running);
        std::vector<HouseMasterAffiliate> hms = _houseMaster.getAffiliatesByLocation(location);
        HouseMasterAffiliate selection{};
        std::map<std::string, std::function<void()>> options{};
        for (const auto &i : hms) {
            options.insert(std::pair<std::string, std::function<void()>>(i.getAffiliateName(), [&]() {
                show(i);
            }));
        }
        Menu affiliates("See details", options);
        while (innerRunning) {
            affiliates.show();
            affiliates.select();
            affiliates.execute(innerRunning);
            std::cin.ignore();
        }
    }}, {"Filter by responsible", [&](){
        bool innerRunning = true;
        std::string responsible = selectResponsible(innerRunning);
        std::vector<HouseMasterAffiliate> hms = _houseMaster.getAffiliatesByResponsible(responsible);
        HouseMasterAffiliate selection{};
        std::map<std::string, std::function<void()>> options{};
        for (const auto &i : hms) {
            options.insert(std::pair<std::string, std::function<void()>>(i.getAffiliateName(), [&]() {
                show(i);
            }));
        }
        Menu affiliates("See details", options);
        affiliates.show();
        affiliates.select();
        affiliates.execute(running);
        std::cin.ignore();
    }}});
    start.show();
    start.select();
    start.execute(running);
}


std::string HousemasterInterface::selectLocation(bool &running) {
    auto location = _houseMaster.getLocations();
    std::string selection{};
    std::map<std::string, std::function<void()>> options{};
    for (const auto &i : location) {
        options.insert(std::pair<std::string, std::function<void()>>(i, [&selection, &i]() {
            selection = i;
        }));
    }
    Menu servicesMenu("Select a Location", options);
    servicesMenu.show();
    servicesMenu.select();
    servicesMenu.execute(running);
    return selection;
}


std::string HousemasterInterface::selectResponsible(bool &running) {
    auto responsible = _houseMaster.getResponsibles();
    std::string selection{};
    std::map<std::string, std::function<void()>> options{};
    for (const auto &i : responsible) {
        options.insert(std::pair<std::string, std::function<void()>>(i, [&selection, &i]() {
            selection = i;
        }));
    }
    Menu servicesMenu("Select a Responsible", options);
    servicesMenu.show();
    servicesMenu.select();
    servicesMenu.execute(running);
    return selection;
}


void HousemasterInterface::show(const HouseMasterAffiliate& affiliate) {
    std::cout << " ____________________HOUSE MASTER____________________ " << std::endl;
    std::cout << "| " << std::setw(50) << std::right << affiliate.getAffiliateName() << " |" << std::endl;
    std::cout << "|                                                    |" << std::endl;
    std::cout << "| [" << "Location" << "] " << std::setw(39) << std::right << affiliate.getLocation() << " |" << std::endl;
    std::cout << "| [" << "Responsible" << "] " << std::setw(36) << std::right << affiliate.getAdmin().getName() << " |"
              << std::endl;
    std::cout << "| [" << "Earnings" << "] " << std::setw(39) << std::right << affiliate.getEarnings() << " |"
              << std::endl;
    std::cout << "|                                                    |" << std::endl;
    std::cout << "| [Enter] Go Back                                    |" << std::endl;
    std::cout << "|____________________________________________________|" << std::endl;
    std::cin.ignore();
}

void HousemasterInterface::showTotalFinances(const HouseMaster &hm){
    std::cout << " ____________________HOUSE MASTER____________________ " << std::endl;
    std::cout << "|                                                    |" << std::endl;
    std::cout << "|  " << "Total" << std::setw(46) << "|" << std::endl;
    std::cout << "| " << " Finances"  << std::setw(41) << std::right << hm.getTotalFinances() << " |" << std::endl;
    std::cout << "|                                                    |" << std::endl;
    std::cout << "| [Enter] Go Back                                    |" << std::endl;
    std::cout << "|____________________________________________________|" << std::endl;
    std::cin.ignore();
}
