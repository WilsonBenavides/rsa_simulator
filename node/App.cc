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

    cMessage *generatePacket;
    long pkCounter;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
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
    pkCounter = 0;

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

        char msgname[20];
        int src = getParentModule()->getIndex(); // our module index
        int size = getParentModule()->getVectorSize() - 1;
        int rdst = intuniform(0, size);
        int dst = rdst != src ? rdst : intuniform(0, size);
        int ns = slotRandomSize;

        sprintf(msgname, "%i-%i-ns%i", src, dst, ns);
        OpticalMsg *opmsg = new OpticalMsg(msgname);
        opmsg->setSrcAddr(src);
        opmsg->setDestAddr(dst);
        opmsg->setSlotReq(ns);
        opmsg->setMsgState(LIGHTPATH_REQUEST);
        opmsg->setRed(intuniform(0, 255));
        opmsg->setGreen(intuniform(0, 255));
        opmsg->setBlue(intuniform(0, 255));

        send(opmsg, "out");

        scheduleAt(simTime() + sendIATime->doubleValue(), generatePacket);
        if (hasGUI())
            getParentModule()->bubble("Generating packet...");
    }
    else {
        // Handle incoming packet
        OpticalMsg *pk = check_and_cast<OpticalMsg*>(msg);
//        EV << "received packet " << pk->getName() << endl;
        EV << "end to end delay : " << simTime() - pk->getCreationTime();
        delete pk;

        if (hasGUI())
            getParentModule()->bubble("Arrived!");
    }
}

