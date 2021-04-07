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
    int numPaths = 2;
    struct Route
    {
        double length;
        std::vector<cTopology::LinkOut*> path;
    };
    std::vector<LinkData> LinkTable;
    std::vector<Route> RouteList;

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

    cTopology *topo = new cTopology("topo");
    topo->extractByModulePath(cStringTokenizer("**.node[*]").asVector());

    cTopology::Node *srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", 0));
    cTopology::Node *targetNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", 5));

    topo->calculateUnweightedSingleShortestPathsTo(targetNode);
    Route routeOne;
    routeOne.length = srcNode->getDistanceToTarget();

    while (srcNode != topo->getTargetNode()) {
        EV << "01 We are in " << srcNode->getModule()->getFullPath() << endl;
        EV << srcNode->getDistanceToTarget() << " hops to go\n";
        cTopology::LinkOut *path = srcNode->getPath(0);
        routeOne.path.push_back(path);
        EV << "Taking gate " << path->getLocalGate()->getFullName() << " we arrive in " << path->getRemoteNode()->getModule()->getFullPath() << " on its gate " << path->getRemoteGate()->getFullName() << endl;
        srcNode = path->getRemoteNode();
    }

    EV << "path lenght one : " << routeOne.length << endl;
    for (cTopology::LinkOut *tmp : routeOne.path) {
        EV << "Reading Links Route One : " << tmp->getLocalNode()->getModule()->getFullPath() << endl;
        cDisplayString &dpStr = tmp->getLocalGate()->getDisplayString();
        dpStr.parse("ls=#00FF00, 3");
    }

    Route routeTwo;
    srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", 0));
    cTopology::Node *disableOne = routeOne.path.at(1)->getLocalNode();
    EV << "02 disabled node one : " << disableOne->getModule()->getFullPath() << endl;
    disableOne->disable();
    topo->calculateUnweightedSingleShortestPathsTo(targetNode);
    routeTwo.length = srcNode->getDistanceToTarget();

    while (srcNode != topo->getTargetNode()) {
        EV << "02 We are in " << srcNode->getModule()->getFullPath() << endl;
        EV << srcNode->getDistanceToTarget() << " hops to go\n";
        cTopology::LinkOut *path = srcNode->getPath(0);
        routeTwo.path.push_back(path);
        EV << "Taking gate " << path->getLocalGate()->getFullName() << " we arrive in " << path->getRemoteNode()->getModule()->getFullPath() << " on its gate " << path->getRemoteGate()->getFullName() << endl;
        srcNode = path->getRemoteNode();
    }

    EV << "path lenght two : " << routeTwo.length << endl;
    for (cTopology::LinkOut *tmp : routeTwo.path) {
        EV << "02 Reading Links Route Two : " << tmp->getLocalNode()->getModule()->getFullPath() << endl;
        cDisplayString &dpStr = tmp->getLocalGate()->getDisplayString();
        dpStr.parse("ls=#FF0000, 3");
    }

    Route routeThree;
    srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", 0));
    cTopology::Node *disableTwo = routeTwo.path.at(1)->getLocalNode();
    EV << "03 disabled node two : " << disableTwo->getModule()->getFullPath() << endl;
    disableTwo->disable();
    topo->calculateUnweightedSingleShortestPathsTo(targetNode);
    routeThree.length = srcNode->getDistanceToTarget();

    while (srcNode != topo->getTargetNode()) {
        EV << "03 We are in " << srcNode->getModule()->getFullPath() << endl;
        EV << srcNode->getDistanceToTarget() << " hops to go\n";
        cTopology::LinkOut *path = srcNode->getPath(0);
        routeThree.path.push_back(path);
        EV << "Taking gate " << path->getLocalGate()->getFullName() << " we arrive in " << path->getRemoteNode()->getModule()->getFullPath() << " on its gate " << path->getRemoteGate()->getFullName() << endl;
        srcNode = path->getRemoteNode();
    }

    EV << "path lenght three : " << routeThree.length << endl;
    for (cTopology::LinkOut *tmp : routeThree.path) {
        EV << "03 Reading Links Route Three : " << tmp->getLocalNode()->getModule()->getFullPath() << endl;
        cDisplayString &dpStr = tmp->getLocalGate()->getDisplayString();
        dpStr.parse("ls=#0000FF, 3");
    }

    cModule *node = getParentModule()->getParentModule()->getSubmodule("node", 0)->getSubmodule("bvwxc");
    sendDirect(msg, node, "directIn");
    delete topo;
}

