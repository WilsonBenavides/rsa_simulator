//
//
#include <iostream>
#include <fstream>
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
    cModule *node = getParentModule()->getParentModule()->getSubmodule("node", 0)->getSubmodule("bvwxc");
    sendDirect(msg, node, "directIn");

    cTopology *topo = new cTopology("topo");

    topo->extractByModulePath(cStringTokenizer("**.node[*]").asVector());
    std::string fileName = "./node/GeneralTableRouting.txt";
    std::ofstream generalRoutingTable(fileName);

    for (int i = 0; i < 3; i++) {
        std::ofstream localRoutingTable("./node/LocalRouting" + std::to_string(i) + ".txt");
        cTopology::Node *srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", i));

        int srcAdd = srcNode->getModule()->par("address");
        EV << "source address : " << srcAdd << endl;
        for (int j = 0; j < topo->getNumNodes(); j++) {
            if (topo->getNode(j) == srcNode)
                continue;  // skip ourselves
            topo->calculateUnweightedSingleShortestPathsTo(topo->getNode(j));

            if (srcNode->getNumPaths() == 0)
                continue;  // not connected

            cGate *parentModuleGate = srcNode->getPath(0)->getLocalGate();

            int srcAddress = srcNode->getModule()->par("address");
            int dstAddress = topo->getNode(j)->getModule()->par("address");
            int gateIndex = parentModuleGate->getIndex();

            generalRoutingTable << srcAddress <<","<< dstAddress <<","<< gateIndex << endl;
            localRoutingTable << dstAddress << "," << gateIndex << endl;
            EV << "  source address : " << srcAddress << " destination address : " << dstAddress << "  gate : " << gateIndex << endl;
        }
        localRoutingTable.close();
    }

    generalRoutingTable.close();
    delete topo;
}

