//
// This file is part of an OMNeT++/OMNEST simulation example.
//

#include <map>
#include <omnetpp.h>
#include "Packet_m.h"

using namespace omnetpp;

/**
 * Demonstrates static routing, utilizing the cTopology class.
 */
class BVWXC : public cSimpleModule
{
private:

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
    if (getId() == 99) {
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
    int n = gateSize("line$o");
    int k = intuniform(0, n - 1);

    EV << "forwarding message " << msg << " on port out[" << k << " ]\n";
    EV << "getId " << getId() << endl;
    EV << "getfullpath " << getFullPath() << endl;
    send(msg, "line$o", k);
}

