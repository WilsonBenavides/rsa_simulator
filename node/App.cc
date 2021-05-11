//
//
#include <sstream>
#include <iostream>
#include <fstream>
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
        opmsg->setKind(intuniform(0, 7));

        send(opmsg, "out");
        numSent++;

        scheduleAt(simTime() + sendIATime->doubleValue(), generatePacket);
//        if (hasGUI())
//            getParentModule()->bubble("Generating packet...");
    }
    else {
        // Handle incoming packet
        OpticalMsg *pk = check_and_cast<OpticalMsg*>(msg);
        int pkid = pk->getId();
        EV << "end to end delay : " << simTime() - pk->getCreationTime() << " | packid : " << pkid << endl;
        getParentModule()->bubble("received");
        std::ifstream ifs("./node/TableRouting.csv");
        std::ofstream tmp;
        if (ifs.is_open()) {
        tmp.open("./node/tmp.csv");
//            tmp.open("./node/tmp.csv", std::ios_base::app);
            std::string node;
            std::string gate;
            std::string msgid;

            while (ifs.good()) {
                std::getline(ifs, node, ',');
                std::getline(ifs, gate, ',');
                std::getline(ifs, msgid, '\n');

                if (node.length() != 0 && gate.length() != 0 && msgid.length() != 0) {
                    if (pkid != std::stoi(msgid)) {
                        tmp << node << "," << gate << "," << msgid << endl;

                    }
                }
            }
            tmp.close();
            ifs.close();
        }
        std::remove("./node/TableRouting.csv");
        std::rename("./node/tmp.csv", "./node/TableRouting.csv");
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
