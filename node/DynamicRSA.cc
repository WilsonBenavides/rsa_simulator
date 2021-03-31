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
    struct LinkData
    {
        int index;
        int source;
        int destination;
        int gate;
    };
    std::vector<LinkData> LinkTable;

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

    int numLinks = 0;
    for (int i = 0; i < topo->getNumNodes(); i++) {
        cTopology::Node *srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", i));
        int numOutLinks = srcNode->getNumOutLinks();
        for (int j = 0; j < numOutLinks; j++) {
            int src = srcNode->getModule()->par("address");
            int dst = topo->getNode(i)->getLinkOut(j)->getRemoteNode()->getModule()->par("address");
            int gate = srcNode->getLinkOut(j)->getLocalGate()->getIndex();
            LinkData data = { numLinks, src, dst, gate };
            LinkTable.push_back(data);
//            EV << "num of links : " << numLinks << " data.src : " << data.source << " gate : " << data.gate << endl;
            numLinks++;
        }
    }

    for (LinkData tmp : LinkTable) {
        EV << "index : " << tmp.index << " source : " << tmp.source << " destination : " << tmp.destination << " gate : " << tmp.gate << endl;

    }

    int ss = LinkTable.at(0).source;
    int dd = LinkTable.at(0).destination;
    int gg = LinkTable.at(0).gate;

    cTopology::Node *srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", 0));
    cDisplayString &connDispStr = srcNode->getLinkOut(0)->getLocalGate()->getDisplayString();
    connDispStr.parse("ls=#25BEB1, 3");

    cTopology::Node *srcNode1 = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", 5));
    cDisplayString &connDispStr2 = srcNode1->getLinkOut(0)->getLocalGate()->getDisplayString();
    connDispStr2.parse("ls=#25BEB1, 3");


    std::string fileName = "./node/GeneralTableRouting.csv";
    std::ofstream generalRoutingTable(fileName);
    generalRoutingTable << "src" << "," << "dst" << "," << "gate" << endl;

    for (int i = 0; i < topo->getNumNodes(); i++) {
        std::ofstream localRoutingTable("./node/LocalRouting" + std::to_string(i) + ".csv");
        localRoutingTable << "dst" << "," << "gate" << endl;

        cTopology::Node *srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", i));

        int srcAdd = srcNode->getModule()->par("address");
//        EV << "source address : " << srcAdd << endl;
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

            generalRoutingTable << srcAddress << "," << dstAddress << "," << gateIndex << endl;
            localRoutingTable << dstAddress << "," << gateIndex << endl;
//            EV << "  source address : " << srcAddress << " destination address : " << dstAddress << "  gate : " << gateIndex << endl;
        }
        localRoutingTable.close();
    }

    generalRoutingTable.close();
    delete topo;
}

