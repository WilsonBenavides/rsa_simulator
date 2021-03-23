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
//    EV << "num of nodes : " << topo->getNumNodes() << endl;

//    EV << "getFullName nodo :  " << srcNode->getModule()->getFullName() << " \n";

//    FILE *GeneralRoutingTable;
    std::string fileName = "./node/GeneralTableRouting.txt";
    std::ofstream generalRoutingTable(fileName);

//    FILE *LocalRoutingTable[topo->getNumNodes()];
//    char rtnames[] = "./node/LocalRouting0000.txt";

    for (int i = 0; i < 3; i++) {
        std::ofstream localRoutingTable("./node/LocalRouting" + std::to_string(i) + ".txt");
        cTopology::Node *srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", i));
//        sprintf(rtnames, "./node/LocalRouting%i.txt");
//        LocalRoutingTable[i] = fopen(rtnames, "a");

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

//        rtable[address] = gateIndex;
            generalRoutingTable << srcAddress <<","<< dstAddress <<","<< gateIndex << endl;
            localRoutingTable << dstAddress << "," << gateIndex << endl;
//            fprintf(generalRoutingTable, "%i,%i,%i\n", srcAddress, dstAddress, gateIndex);
//            fprintf(LocalRoutingTable[i], "%i,%i\n", dstAddress, gateIndex);

            EV << "  source address : " << srcAddress << " destination address : " << dstAddress << "  gate : " << gateIndex << endl;
        }
//        fclose(LocalRoutingTable[i]);
        localRoutingTable.close();
    }

//    int k = srcNode->getModule()->gateSize("portOut");
//
//    for (int i = 0; i < k; i++) {
//        cGate *g = srcNode->getModule()->gate("portOut", i);
//        cDisplayString &connDispStr = g->getDisplayString();
//        connDispStr.parse("ls=#25BEB1, 5");
//    }

//    fclose(generalRoutingTable);
    generalRoutingTable.close();
    delete topo;
}

