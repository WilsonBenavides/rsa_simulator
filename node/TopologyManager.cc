//
//
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <iomanip>
#include <omnetpp.h>
#include "OpticalMsg_m.h"
#include "Utils.h"

using namespace omnetpp;

/**
 *
 */
class TopologyManager : public cSimpleModule
{
private:
    int numPaths;

public:
    TopologyManager();
    virtual ~TopologyManager();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(TopologyManager);

TopologyManager::TopologyManager()
{
}

TopologyManager::~TopologyManager()
{
}

void TopologyManager::initialize()
{
    numPaths = par("numPaths");
}

void TopologyManager::handleMessage(cMessage *msg)
{
//    int numPaths = 1;
    int pathFound = 0;

    OpticalMsg *rcvMsg = check_and_cast<OpticalMsg*>(msg);
    int src = rcvMsg->getSrcAddr();
    int dst = rcvMsg->getDestAddr();
    int state = rcvMsg->getMsgState();
    int numSlots = rcvMsg->getSlotReq();
    int red = rcvMsg->getRed();
    int green = rcvMsg->getGreen();
    int blue = rcvMsg->getBlue();

    if (state == LIGHTPATH_REQUEST) {
    }
    std::stringstream stream;
    stream << "ls=#";
    stream << std::hex << red;
    stream << std::hex << green;
    stream << std::hex << blue;
    stream << ",5";
    std::string result(stream.str());
//    EV << "stream string.............. : " << result << endl;

    cTopology *topo = new cTopology("topo");
    std::vector<std::string> nedTypes;
    nedTypes.push_back(getParentModule()->getParentModule()->getSubmodule("node", 0)->getNedTypeName());
    topo->extractByNedTypeName(nedTypes);

    for (int i = 0; i < topo->getNumNodes(); i++) {
        cTopology::Node *srcN = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", i));
        int numOutLinks = srcN->getNumOutLinks();
        for (int j = 0; j < numOutLinks; j++) {
            cDisplayString &dStr = srcN->getLinkOut(j)->getLocalGate()->getDisplayString();
            dStr.parse("ls=#939393,1");
        }
    }

    cTopology::Node *srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", src));
    cTopology::Node *targetNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", dst));

    std::vector<cTopology::LinkIn*> route;
    std::vector<cTopology::LinkIn*> path;
    std::vector<std::vector<cTopology::LinkIn*>> pathList;
    std::vector<std::vector<cTopology::LinkIn*>> routeList;
    std::vector<int> nodeDis;
    for (int i = 0; i < topo->getNumNodes(); i++) {
        nodeDis.push_back(MAXIMUM);
        path.push_back(nullptr);
    }
    nodeDis.at(targetNode->getModule()->getIndex()) = 0;
//    EV << "node target zero : " << nodeDis.at(targetNode->getModule()->getIndex()) << endl;
    std::deque<cTopology::Node*> q;
    q.push_back(targetNode);
    while (!q.empty()) {
        cTopology::Node *v = q.front();
        q.pop_front();
        for (int i = 0; i < v->getNumInLinks(); i++) {
            cTopology::Node *w;
            w = v->getLinkIn(i)->getRemoteNode();
//            cDisplayString &dispStr = v->getLinkIn(i)->getRemoteGate()->getDisplayString();
//            dispStr.parse("ls=#F00000, 6");

            if (w == srcNode) {
                pathFound++;
                nodeDis.at(w->getModule()->getIndex()) = 1 + nodeDis.at(v->getModule()->getIndex());
                path.at(w->getModule()->getIndex()) = v->getLinkIn(i);
                pathList.push_back(path);
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

    while (srcNode != targetNode) {
        for (int i = 0; i < topo->getNumNodes(); i++) {
            if (path[i] != nullptr) {
                if (srcNode == path[i]->getRemoteNode()) {
                    route.push_back(path[i]);
//                    EV << "srcNode = path_i_getRemoteNode  :  " << path[i]->getRemoteNode()->getModule()->getFullName() << endl;
                    srcNode = path[i]->getLocalNode();
//                    EV << "getLocalNode  :  " << path[i]->getLocalNode()->getModule()->getFullName() << endl;
                }
//                EV << "i remoteNode : " << path[i]->getRemoteNode()->getModule()->getFullName() << "  : distance : " << nodeDis[i] << endl;
            }
        }
    }
    for (int i = 0; i < pathList.size(); i++) {
        srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", src));
        EV << "path list size : " << pathList.size() << endl;
        routeList.push_back(std::vector<cTopology::LinkIn*>());
        while (srcNode != targetNode) {
            for (int j = 0; j < pathList[i].size(); j++) {
                if (pathList[i][j] != nullptr) {
                    if (srcNode == pathList[i][j]->getRemoteNode()) {
                        EV << "src node found " << pathList[i][j]->getRemoteNode()->getModule()->getFullName() << endl;
                        routeList[i].push_back(pathList[i][j]);
                        srcNode = pathList[i][j]->getLocalNode();
                    }
                }
            }
        }
    }

    std::vector<const char*> cols = { "ls=#ffffff,5", "ls=#ffffff,5", "ls=#999999,5", "ls=#666666,5", "ls=#333333,5" };
    for (int i = 0; i < routeList.size(); i++) {
        for (int j = 0; j < routeList[i].size(); j++) {
            cDisplayString &dispStr = routeList[i][j]->getRemoteGate()->getDisplayString();
            dispStr.parse(cols[i]);
        }
    }

    OpticalMsgPath *opticalPath = new OpticalMsgPath("Optical Message Path");
    opticalPath->setSrcAddr(src);
    opticalPath->setDestAddr(dst);
    opticalPath->setSlotReq(numSlots);
    opticalPath->setMsgState(LIGHTPATH_PROCESSING);
    opticalPath->setOpticalPathArraySize(route.size());
    opticalPath->setRed(red);
    opticalPath->setGreen(green);
    opticalPath->setBlue(blue);

    int index = 0;
    std::string fileName = "./node/TableRouting.csv";
    std::ofstream routingTable(fileName);

    for (cTopology::LinkIn *tmp : routeList[0]) {
        cDisplayString &dispStr = tmp->getRemoteGate()->getDisplayString();
        dispStr.parse(result.c_str());
        int gate = tmp->getRemoteGate()->getIndex();
        int node = tmp->getRemoteNode()->getModule()->getIndex();
        int id = tmp->getRemoteGate()->getConnectionId();
        opticalPath->setOpticalPath(index, id);
        routingTable << node << "," << gate << endl;
        index++;
    }
    routingTable.close();

    delete rcvMsg;
    delete topo;
    cModule *spec = getParentModule()->getSubmodule("topology");
    sendDirect(opticalPath, spec, "directTopoOut");

}

