//
//

#include <vector>
#include <string>
#include <omnetpp.h>
#include "TopologyManager.h"

using namespace omnetpp;

/**
 * Generates traffic for the network.
 */
class DynamicRSA : public cSimpleModule
{
public:
    DynamicRSA();
    virtual ~DynamicRSA();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(DynamicRSA);

DynamicRSA::DynamicRSA()
{
}

DynamicRSA::~DynamicRSA()
{
}

void DynamicRSA::initialize()
{
    TopologyManager *tm;
    EV << "topology manager called, num of nodes " << tm->getNumNodes() << endl;
}

void DynamicRSA::handleMessage(cMessage *msg)
{
    EV << "message arrived" << endl;
    cModule *core = getParentModule()->getParentModule()->getSubmodule("seattle")->getSubmodule("bvwxc");
    EV << "bvwxc ???    " << core->getName() << endl;
    sendDirect(msg, core, "in");
}

