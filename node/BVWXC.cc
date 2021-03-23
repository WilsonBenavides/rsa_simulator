//
// This file is part of an OMNeT++/OMNEST simulation example.
//
#include <sstream>
#include <iostream>
#include <fstream>
#include <omnetpp.h>
#include "OpticalMsg_m.h"

using namespace omnetpp;

/**
 * Demonstrates static routing, utilizing the cTopology class.
 */
class BVWXC : public cSimpleModule
{
private:
    typedef std::map<int, int> RoutingTable;  // destaddr -> gateindex
    RoutingTable rtable;

protected:
    virtual void forwardMessage(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(BVWXC);

void BVWXC::initialize()
{
}

void BVWXC::handleMessage(cMessage *msg)
{
    int index = getParentModule()->par("address");
    if (index == 10) {
        //Message arrived
        EV << "Message : " << msg << "  arrived!\n";
        delete msg;
    }
    else {
        forwardMessage(msg);
    }
}

void BVWXC::forwardMessage(cMessage *msg)
{
//    int n = gateSize("line$o");
//    int k = intuniform(0, n - 1);

//    EV << "forwarding message " << msg << " on port out[" << k << " ]\n";
//    EV << "getId " << getId() << endl;
//    EV << "getfullpath " << getFullPath() << endl;
//    send(msg, "line$o", k);

//    EV << "msg send to controller : " << getParentModule()->getParentModule()->getSubmodule("controller")->getName() << endl;
//    cModule *control = getParentModule()->getParentModule()->getSubmodule("controller");
//    sendDirect(msg, control, "in");

    OpticalMsg *opmsg = check_and_cast<OpticalMsg*>(msg);
    int srcAddress = opmsg->getSrcAddr();
    int dstAddress = opmsg->getDestAddr();
    std::vector<std::string> dstGate;
    EV << "source address " << srcAddress << endl;

    std::string myText;
    std::ifstream myReadFile("./node/LocalRouting0.txt");

    while (std::getline(myReadFile, myText)) {
        EV << myText << endl;
        std::stringstream ss(myText);
        std::string tmp;
        int dstaddr = 0;
        int gate = 0;
        while (std::getline(ss, tmp, ',')) {
            dstGate.push_back(tmp);

        }
    }

    for (auto word : dstGate) {
        EV << "word  : " << word << endl;
    }
    myReadFile.close();

}

