//
//
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <omnetpp.h>
#include "OpticalMsg_m.h"

#define LIGHTPATH_REQUEST 0
#define LIGHTPATH_PROCESSING 1
#define LIGHTPATH_ASSIGNMENT 2
#define MAXIMUM 1000

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
    struct LinksByLength
    {
        std::vector<int> length;
        std::vector<cTopology::LinkIn*> path;
    };
    struct NodeVisited
    {
        cTopology::Node *nodesVisited;
        int numHops;
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

    OpticalMsg *rcvMsg = check_and_cast<OpticalMsg*>(msg);
    int src = rcvMsg->getSrcAddr();
    int dst = rcvMsg->getDestAddr();
    int state = rcvMsg->getMsgState();

    if (state == LIGHTPATH_REQUEST) {
        EV << "msg request type " << endl;
    }

    cTopology *topo = new cTopology("topo");
    topo->extractByModulePath(cStringTokenizer("**.node[*]").asVector());

    cTopology::Node *srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", src));
    cTopology::Node *targetNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", dst));

    std::deque<cTopology::Node*> q;
    q.push_back(targetNode);

    int flag = 0;
    int numPaths = 1;
    int counter = 0;
    int pathFound = 0;
    std::vector<NodeVisited> nodeVisited;

    std::vector<LinksByLength> *dataLinks;
    while (!q.empty()) {
//        EV << "nodes : " << counter++ << endl;

        cTopology::Node *v = q.front();
        q.pop_front();
        if (v == srcNode || (v == targetNode && counter > 2)) {
//            EV << "same to source.. better discard" << endl;
            q.pop_front();
            v = q.front();
        }
        NodeVisited nv;
        nv.nodesVisited = v;
        nv.numHops = counter;
        nodeVisited.push_back(nv);

        LinksByLength *lbl;
        counter++;
        for (int i = 0; i < v->getNumInLinks(); i++) {
            cTopology::Node *w;
            w = v->getLinkIn(i)->getRemoteNode();
            cDisplayString& dispStr = v->getLinkIn(i)->getRemoteGate()->getDisplayString();
            dispStr.parse("ls=#F00000, 6");

//            lbl->path.push_back(v->getLinkIn(i));
//            lbl->length.push_back(counter);

            if (w == srcNode) {
                pathFound++;
                EV << " ::::::::::::::::::::::::::::::::::::::::::::..found!!!!!!:::::::::::::::::::::::" << endl;
                break;
            }
            if (w == targetNode) {
//                EV << "same the dest, best ignore" << endl;
                break;
            }
            q.push_back(w);
//            EV << "push back v node : " << v->getModule()->getFullName() << "   ::   w node : " << w->getModule()->getFullName() << endl;

            if (pathFound == numPaths)
                break;
        }
//        dataLinks->push_back(lbl);
        if (pathFound == numPaths)
            break;
    }

    for (NodeVisited nv : nodeVisited) {
//        EV << "Nodes in stack ::  " << nv.nodesVisited->getModule()->getFullName() << endl;
//        EV << "dist  ::  " << nv.numHops << endl;
    }

//    for (int i = 0; i < dataLinks[0].path.size(); i++) {
////        EV << "links 1  :  " << dataLinks[0].length.at(i) << endl;
//        cDisplayString &dispStr = dataLinks[0].path.at(i)->getLocalGate()->getDisplayString();
////        dispStr.parse("ls=#25BEB1, 6");
//    }
    cModule *node = getParentModule()->getParentModule()->getSubmodule("node", 0)->getSubmodule("bvwxc");
    sendDirect(msg, node, "directIn");
    delete topo;
}

