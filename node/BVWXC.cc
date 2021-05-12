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

//        std::ofstream tmp;
//        if (ifs.is_open()) {
//            tmp.open("./node/tmpBVWXC.csv");
//            std::string node;
//            std::string gate;
//            std::string msgid;
//            std::string lnkid;
//            std::string color;
//            std::string slot;
//
//            while (ifs.good()) {
//                std::getline(ifs, node, ',');
//                std::getline(ifs, gate, ',');
//                std::getline(ifs, msgid, ',');
//                std::getline(ifs, lnkid, ',');
//                std::getline(ifs, color, ',');
//                std::getline(ifs, slot, '\n');
//
//                if (node.length() != 0 && gate.length() != 0 && msgid.length() != 0 && lnkid.length() != 0 && color.length() != 0 && slot.length() != 0) {
//                    if (true) {
//                        tmp << node << "," << gate << "," << msgid << "," << lnkid << "," << color << "," << slotFF << endl;
//
//                    }
//                }
//            }
//            tmp.close();
//            ifs.close();
//        }
//        std::remove("./node/TableRouting.csv");
//        std::rename("./node/tmpBVWXC.csv", "./node/TableRouting.csv");
//
//        ifs.open("./node/TableRouting.csv");
        if (ifs.is_open()) {
            std::string node;
            std::string gate;
            std::string msgid;
            std::string lnkid;
            std::string color;
            std::string slot;

            while (ifs.good()) {
                std::getline(ifs, node, ',');
                std::getline(ifs, gate, ',');
                std::getline(ifs, msgid, ',');
                std::getline(ifs, lnkid, ',');
                std::getline(ifs, color, ',');
                std::getline(ifs, slot, '\n');

                if (node.length() != 0 && gate.length() != 0 && msgid.length() != 0 && lnkid.length() != 0 && color.length() != 0 && slot.length() != 0) {
                    if (getParentModule()->getIndex() == std::stoi(node) && std::stoi(msgid) == id) {
                        send(msg, "out", std::stoi(gate));
                    }
                }
            }
            ifs.close();
        }

    }
}

