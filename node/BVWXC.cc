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

    if (opmsg->getDestAddr() == ss) {
        //Message arrived
        send(msg, "localOut");
    }
    else {
        forwardMessage(msg);
    }
}

void BVWXC::forwardMessage(cMessage *msg)
{

    OpticalMsg *rcvMsg = check_and_cast<OpticalMsg*>(msg);
    int src = rcvMsg->getSrcAddr();
    int dst = rcvMsg->getDestAddr();
    int id = rcvMsg->getId();
    int state = rcvMsg->getMsgState();

    if (state == LIGHTPATH_REQUEST) {
        cModule *control = getParentModule()->getParentModule()->getSubmodule("controller");
        sendDirect(msg, control, "in");
    }
    if (state == LIGHTPATH_ASSIGNMENT) {
        std::ifstream ifs("./node/TableRouting.csv");
        if (ifs.is_open()) {
            std::string node;
            std::string gate;
            std::string msgid;

            while (ifs.good()) {
                std::getline(ifs, node, ',');
                std::getline(ifs, gate, ',');
                std::getline(ifs, msgid, '\n');

//                EV << "node : " << node << " | gate : " << gate << " | msgid : " << msgid << endl;

                if (node.length() != 0 && gate.length() != 0 && msgid.length() != 0) {
                    if (getParentModule()->getIndex() == std::stoi(node) && std::stoi(msgid) == id) {
                        send(msg, "out", std::stoi(gate));
                    }
                }
            }
            ifs.close();
        }
    }
}

