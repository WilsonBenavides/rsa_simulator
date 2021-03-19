//
//

#include <vector>
#include <string>
#include <omnetpp.h>

using namespace omnetpp;

/**
 * Generates traffic for the network.
 */
class TopologyManager : public cSimpleModule
{
public:
    TopologyManager();
    virtual ~TopologyManager();

protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override; // NOTE THE const MODIFIER!!!

    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(TopologyManager);

int TopologyManager::numInitStages() const
{
    return 3;
}

TopologyManager::TopologyManager()
{
}

TopologyManager::~TopologyManager()
{
}

void TopologyManager::initialize(int stage)
{
    if (stage == 2) {
        EV << " Topology Manager, stage 2 " << endl;
    }
}

void TopologyManager::handleMessage(cMessage *msg)
{
}

