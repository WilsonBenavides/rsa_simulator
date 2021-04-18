//
// This file is part of an OMNeT++/OMNEST simulation example.
//
#include <sstream>
#include <iostream>
#include <fstream>
#include <omnetpp.h>
#include "OpticalMsg_m.h"
#include "Utils.h"

using namespace omnetpp;

/**
 * Demonstrates static routing, utilizing the cTopology class.
 */
class BVWXC : public cSimpleModule
{
private:
    int myAddress;
    typedef std::map<int, int> RoutingTable;  // destaddr -> gateindex

protected:
    virtual void forwardMessage(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(BVWXC);

void BVWXC::initialize()
{
    myAddress = getParentModule()->par("address");
}

void BVWXC::handleMessage(cMessage *msg)
{
    OpticalMsg *opmsg = check_and_cast<OpticalMsg*>(msg);
    int ss = getParentModule()->par("address");

//    EV << "address : " << ss << endl;
    if (opmsg->getDestAddr() == ss) {
        //Message arrived
        bubble("message arrived!");
        send(msg, "localOut");
//        delete msg;
    }
    else {
        forwardMessage(msg);
    }
}

void BVWXC::forwardMessage(cMessage *msg)
{

//    EV << "msg send to controller : " << getParentModule()->getParentModule()->getSubmodule("controller")->getName() << endl;
    OpticalMsg *rcvMsg = check_and_cast<OpticalMsg*>(msg);
    int src = rcvMsg->getSrcAddr();
    int dst = rcvMsg->getDestAddr();
    int state = rcvMsg->getMsgState();

    if (state == LIGHTPATH_REQUEST) {
//        EV << "LIGHTPATH_REQUEST " << endl;
        cModule *control = getParentModule()->getParentModule()->getSubmodule("controller");
        sendDirect(msg, control, "in");
    }
    if (state == LIGHTPATH_ASSIGNMENT) {
//        EV << "LIGHTPATH_ASSIGNMENT " << endl;
        std::string line;
        std::ifstream ifs("./node/TableRouting.csv");
        if (ifs.is_open()) {
            while (std::getline(ifs, line)) {
//                EV << line << endl;
                int index = line.find(',');
                int node = std::stoi(line.substr(0, index++));
                int gate = std::stoi(line.substr(index, line.size()));

                if (getParentModule()->getIndex() == node) {
                    send(msg, "out", gate);
//                    EV << "message sent.." << endl;
                }
            }
            ifs.close();
        }
    }
}

