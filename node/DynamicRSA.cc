//
//

#include <vector>
#include <string>
#include <omnetpp.h>

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
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override; // NOTE THE const MODIFIER!!!

    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(DynamicRSA);

int DynamicRSA::numInitStages() const
{
    return 3;
}

DynamicRSA::DynamicRSA()
{
}

DynamicRSA::~DynamicRSA()
{
}

void DynamicRSA::initialize(int stage)
{
}

void DynamicRSA::handleMessage(cMessage *msg)
{
    EV << "message arrived" << endl;
    cModule *core = getParentModule()->getParentModule()->getSubmodule("seattle")->getSubmodule("bvwxc");
    EV << "bvwxc ???    " << core->getName() << endl;
    sendDirect(msg, core, "in");
}

