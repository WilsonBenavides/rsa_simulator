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
//        EV << "msg request type " << endl;
    }

    cTopology *topo = new cTopology("topo");
    topo->extractByModulePath(cStringTokenizer("**.node[*]").asVector());

    cTopology::Node *srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", src));
    cTopology::Node *targetNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", dst));

    int flag = 0;
    int numPaths = 1;
    int counter = 0;
    int pathFound = 0;

    std::vector<cTopology::LinkIn*> routeOne;
    std::vector<cTopology::LinkIn*> path;
    std::vector<int> nodeDis;
    for (int i = 0; i < topo->getNumNodes(); i++) {
        nodeDis.push_back(MAXIMUM);
        path.push_back(nullptr);
    }
    nodeDis.at(targetNode->getModule()->getIndex()) = 0;
//    EV << "node target zero : " << nodeDis.at(targetNode->getModule()->getIndex()) << endl;
    std::deque<cTopology::Node*> q;
    q.push_back(targetNode);
    std::vector<LinksByLength> *dataLinks;
    while (!q.empty()) {
        cTopology::Node *v = q.front();
        q.pop_front();
        counter++;
        for (int i = 0; i < v->getNumInLinks(); i++) {
            cTopology::Node *w;
            w = v->getLinkIn(i)->getRemoteNode();
//            cDisplayString &dispStr = v->getLinkIn(i)->getRemoteGate()->getDisplayString();
//            dispStr.parse("ls=#F00000, 6");

            if (w == srcNode) {
                pathFound++;
                nodeDis.at(w->getModule()->getIndex()) = 1 + nodeDis.at(v->getModule()->getIndex());
                path.at(w->getModule()->getIndex()) = v->getLinkIn(i);
//                EV << " ::::::::::::::::::::::::::::::::::::::::::::..found!!!!!!:::::::::::::::::::::::" << endl;
                break;
            }
            int dist = nodeDis.at(w->getModule()->getIndex());
//            EV << "max : " << MAXIMUM << "  dist  :  " << dist << endl;
            if (dist == MAXIMUM) {
//                EV << "inside if dist == MAXIMUM .. " << endl;
//                EV << "::V:: node : " << v->getModule()->getFullName() << "   ::W:: node : " << w->getModule()->getFullName() << endl;
//                EV << "table...................." << endl;
                for (int i : nodeDis) {
//                    EV << "index : " << i << endl;
                }
                nodeDis.at(w->getModule()->getIndex()) = 1 + nodeDis.at(v->getModule()->getIndex());
                path.at(w->getModule()->getIndex()) = v->getLinkIn(i);
                q.push_back(w);
            }

            if (pathFound == numPaths)
                break;
        }
//        dataLinks->push_back(lbl);
        if (pathFound == numPaths)
            break;
    }
    for (int i : nodeDis) {
//        EV << "index : " << i << endl;
    }

    while (srcNode != targetNode) {
        for (int i = 0; i < topo->getNumNodes(); i++) {
            if (path[i] != nullptr) {
                if (srcNode == path[i]->getRemoteNode()) {
                    routeOne.push_back(path[i]);
//                    EV << "srcNode = path_i_getRemoteNode  :  " << path[i]->getRemoteNode()->getModule()->getFullName() << endl;
                    srcNode = path[i]->getLocalNode();
//                    EV << "getLocalNode  :  " << path[i]->getLocalNode()->getModule()->getFullName() << endl;
                }
//                EV << "i remoteNode : " << path[i]->getRemoteNode()->getModule()->getFullName() << "  : distance : " << nodeDis[i] << endl;
            }
        }
    }
    std::string fileName = "./node/TableRouting.csv";
    std::ofstream routingTable(fileName);
    for (cTopology::LinkIn *tmp : routeOne) {
        cDisplayString &dispStr = tmp->getRemoteGate()->getDisplayString();
        dispStr.parse("ls=#F00000, 6");
        int gate = tmp->getRemoteGate()->getIndex();
        int node = tmp->getRemoteNode()->getModule()->getIndex();
//        EV << "node : " << node << "    gate  : " << gate << endl;
        routingTable << node << "," << gate << endl;
    }
    routingTable.close();
    rcvMsg->setMsgState(LIGHTPATH_ASSIGNMENT);

//    for (int i = 0; i < dataLinks[0].path.size(); i++) {
////        EV << "links 1  :  " << dataLinks[0].length.at(i) << endl;
//        cDisplayString &dispStr = dataLinks[0].path.at(i)->getLocalGate()->getDisplayString();
////        dispStr.parse("ls=#25BEB1, 6");
//    }
    cModule *node = getParentModule()->getParentModule()->getSubmodule("node", 0)->getSubmodule("bvwxc");
    sendDirect(rcvMsg, node, "directIn");
    delete topo;
}

