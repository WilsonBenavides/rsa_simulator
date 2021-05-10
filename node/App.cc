//
//
#define FSM_DEBUG    // enables debug output from FSMs
#include <vector>
#include <string>
#include <omnetpp.h>
#include "OpticalMsg_m.h"
#include "Utils.h"

using namespace omnetpp;

/**
 * Generates traffic for the network.
 */
class App : public cSimpleModule
{
public:
    App();
    virtual ~App();

private:
    int address;
    int slotRandomSize;
    cPar *sendIATime;
    int64_t packetLength;

    cMessage *generatePacket;
    long numSent;
    long numReceived;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;
    cFigure::Color generateRandomColor();
};

Define_Module(App);

App::App()
{
    generatePacket = nullptr;
}

App::~App()
{
    cancelAndDelete(generatePacket);
}

void App::initialize()
{
    address = par("address");
    slotRandomSize = par("slotRandomSize");
    sendIATime = &par("sendIaTime");
    packetLength = par("packetLength");


    numSent = 0;
    numReceived = 0;
    WATCH(numSent);
    WATCH(numReceived);

    generatePacket = new cMessage("nextPacket");
    scheduleAt(sendIATime->doubleValue(), generatePacket);
}

void App::handleMessage(cMessage *msg)
{
//    send(msg, "out");
    if (msg == generatePacket) {
        // Sending packet
//        char pkname[40];
//        sprintf(pkname, "%d-to-%d", address, destAddress, pkCounter++);
//        EV << "generating packet " << pkname << endl;

        char msgname[40];
        int src = getParentModule()->getIndex(); // our module index
        int size = getParentModule()->getVectorSize() - 2;
        int dst = intuniform(0, size);
        if (dst >= src)
            dst++;
        int ns = slotRandomSize;

        sprintf(msgname, "%i-%i-ns%i", src, dst, ns);
        OpticalMsg *opmsg = new OpticalMsg(msgname);
        opmsg->setSrcAddr(src);
        opmsg->setDestAddr(dst);
        opmsg->setSlotReq(ns);
        opmsg->setMsgState(LIGHTPATH_REQUEST);
        opmsg->setColor(intrand(16777213));
        opmsg->setByteLength(packetLength);
        EV << "packet length : " << opmsg->getByteLength() << endl;

        send(opmsg, "out");
        numSent++;

        scheduleAt(simTime() + sendIATime->doubleValue(), generatePacket);
//        if (hasGUI())
//            getParentModule()->bubble("Generating packet...");
    }
    else {
        // Handle incoming packet
        OpticalMsg *pk = check_and_cast<OpticalMsg*>(msg);
//        EV << "received packet " << pk->getName() << endl;
        EV << "end to end delay : " << simTime() - pk->getCreationTime() << endl;
        numReceived++;
        delete pk;

//        if (hasGUI())
//            getParentModule()->bubble("Arrived!");
    }
}

void App::refreshDisplay() const
{
    char buf[40];
    sprintf(buf, "r: %ld s: %ld", numReceived, numSent);
    getParentModule()->getDisplayString().setTagArg("t", 0, buf);
}
