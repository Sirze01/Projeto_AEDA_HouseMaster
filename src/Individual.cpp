#include "Individual.h"

#include <utility>
#include <algorithm>
#include <sstream>

Individual::Individual():_displayName(), _id() {}

Individual::Individual(std::string  name) : _displayName(std::move(name)), _id(0) {
}

std::string Individual::getName() {
    return _displayName;
}




// Collaborator associated methods
unsigned Collaborator::_idSeq = 0;

std::vector<Service *> Collaborator::getServices() {
    return _services;
}

int Collaborator::getScore() {
    return _score;
}


Collaborator::Collaborator(std::vector<Service *> functions, const std::string &name, bool pro) : Individual(name),
                                                                                               _services(std::move(functions)),
                                                                                               _score(newHere), _pro(pro) {
    _id = _idSeq++;
}


bool Collaborator::canPreform(Service *service) {
    auto found = std::find_if(_services.begin(), _services.end(), [&service](Service *service1) {
        Service s1 = *service;
        Service s2 = *service1;
        return s1.name == s2.name;
    });
    return found != _services.end();
}

bool Collaborator::operator==(const Collaborator& ind2) const {
    return _id == ind2._id;
}


bool Collaborator::isAvailable(date start, date duration) {
    for (const auto &intervention : getAssociatedInterventions()) {
        if (intervention->conflictsWith(start, duration)) {
            return false;
        }
    }
    return true;
}

void Collaborator::addClassification(Classification classification) {
    _classifications.push_back(classification);
}

void Collaborator::updateScore() {
    double sum{}, average{};
    if(_classifications.empty()) _score = newHere;
    else {
        std::for_each(_classifications.begin(), _classifications.end(), [&] (int n) {
            sum += n;
        });
        average = sum / static_cast<double>(_classifications.size());
        _score = static_cast<Classification>(round(average));
    }
}

bool Collaborator::hasQualificationToPreform(Intervention *intervention) const {
    return !(intervention->getService()->pro || intervention->getForcePro()) || isPro();
}

bool Collaborator::isPro() const {
    return _pro;
}

bool Collaborator::canDo(Intervention *intervention) {
    Service *service = intervention->getService();
    date start = *intervention->getStartingTime();
    date duration = intervention->getService()->duration;
    return isAvailable(start, duration) && canPreform(service) && hasQualificationToPreform(intervention);
}

std::string Collaborator::getId() const {
    std::ostringstream outStr;
    outStr << "collab" << _id;
    return outStr.str();
}



// Client associated methods
unsigned Client::_idSeq = 0;

Client::Client(unsigned int nif, const std::string &name, bool premium)
        : Individual(name), _nif(nif), _requestedInterventions(), _premium(premium) {
    _id = _idSeq++;
}

unsigned int Client::getNif() {
    return (_nif);
}

int Client::requestIntervention(date appointment, Service type) {
    auto intervention = new Intervention(appointment, std::move(type), _premium);
    _requestedInterventions.push_back(intervention);
    return 0;
}

const std::vector<Intervention *> &Client::getRequestedInterventions() const {
    return _requestedInterventions;
}

bool Client::isPremium() const {
    return _premium;
}

std::string Client::getId() const {
    std::ostringstream outStr;
    outStr << "client" << _id;
    return outStr.str();
}

bool Client::operator==(const Client& ind2) const {
    return _id == ind2._id;
}
