#ifndef SRC_INDIVIDUAL_H
#define SRC_INDIVIDUAL_H

#include <string>
#include <vector>
#include <cmath>
#include <numeric>
#include "Services.h"

class Individual {
public:

    Individual();

    explicit Individual(std::string name);

    virtual std::string getId() const = 0;

    std::string getName();


protected:
    std::string _displayName;
    unsigned int _id;
};


// Collaborator associated code
enum Classification {
    unreliable = -2,
    clumsy = -1,
    newHere = 0,
    getsItDone = 1,
    hardWorker = 2,
    attentive = 3,
    savior = 4
};


class Collaborator : public Individual {
public:
    Collaborator(std::vector<Service *> functions, const std::string &name, bool pro);

    std::vector<Service *> getServices();

    bool isPro() const;

    bool canPreform(Service *service);

    bool isAvailable(date start, date duration);

    bool hasQualificationToPreform(Intervention *intervention) const;

    bool canDo(Intervention *intervention);

    int getScore();

    void addClassification(Classification classification);

    void updateScore();

    static unsigned int _idSeq;

    std::string getId() const override;

    bool operator== (const Collaborator& ind2) const;

    //std::vector<Intervention*> getAssociatedInterventions(){return _associatedInterventions};

private:
    //std::vector<Intervention*> _associatedInterventions;
    std::vector<Classification> _classifications;
    std::vector<Service *> _services;
    Classification _score;
    bool _pro;
};


// Client code
class Client : public Individual {
public:
    Client(unsigned int nif, const std::string &name, bool premium);

    unsigned int getNif();

    bool isPremium() const;

    int requestIntervention(date appointment, Service type);

    static unsigned int _idSeq;

    const std::vector<Intervention *> &getRequestedInterventions() const;

    std::string getId() const override;

    bool operator== (const Client& ind2) const;

private:
    unsigned int _nif;
    std::vector<Intervention *> _requestedInterventions;
    bool _premium;
};

#endif
