//
//

#include <vector>
#include <string>
#include <omnetpp.h>

using namespace omnetpp;

/**
 * Generates traffic for the network.
 */
class SpectrumManager : public cSimpleModule
{
public:
    SpectrumManager();
    virtual ~SpectrumManager();

protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override; // NOTE THE const MODIFIER!!!

    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(SpectrumManager);

int SpectrumManager::numInitStages() const
{
    return 3;
}

SpectrumManager::SpectrumManager()
{
}

SpectrumManager::~SpectrumManager()
{
}

void SpectrumManager::initialize(int stage)
{
    if (stage == 1) {
        EV << " Spectrum Manager, stage 1 " << endl;
    }
}

void SpectrumManager::handleMessage(cMessage *msg)
{
}

