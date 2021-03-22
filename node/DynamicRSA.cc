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
}

void DynamicRSA::handleMessage(cMessage *msg)
{
    EV << "message arrived" << endl;
    cModule *core = getParentModule()->getParentModule()->getSubmodule("seattle")->getSubmodule("bvwxc");
    EV << "bvwxc ???    " << core->getName() << endl;
    sendDirect(msg, core, "directIn");

    cTopology *topo = new cTopology("topo");

    topo->extractByModulePath(cStringTokenizer("**.seattle **.palo_alto **.san_diego **.champaign **.salt_lake **.houston **.ann_arbor **.boulder **.lincoln **.atlanta **.college_pk **.pittsburg **.princeton **.ithaca").asVector());
    EV << "num of nodes : " << topo->getNumNodes() << endl;

    cTopology::Node *srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("seattle"));

    EV << "getFullName nodo :  " << srcNode->getModule()->getFullName() << " \n";
    EV << "getIndex nodo:  " << srcNode->getModule()->getIndex() << " \n";
    EV << "gate size :  " << srcNode->getModule()->gateSize("portOut") << " \n";

    int k = srcNode->getModule()->gateSize("portOut");

    for (int i = 0; i < k; i++) {
        cGate *g = srcNode->getModule()->gate("portOut", i);
        cDisplayString &connDispStr = g->getDisplayString();
        connDispStr.parse("ls=#25BEB1, 5");
    }

//    for (int i = 0; i < k2; i++) {
//        cDisplayString &connDispStr = srcNode2->getModule()->gate("port$o", i)->getDisplayString();
//        connDispStr.parse("ls=#25BEB1, 5");
//    }
    delete topo;
}

