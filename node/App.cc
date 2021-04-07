//
//

#include <vector>
#include <string>
#include <omnetpp.h>
#include "OpticalMsg_m.h"

#define LIGHTPATH_REQUEST 0
#define LIGHTPATH_PROCESSING 1
#define LIGHTPATH_ASSIGNMENT 2

using namespace omnetpp;

/**
 * Generates traffic for the network.
 */
class App : public cSimpleModule
{
public:
    App();
    virtual ~App();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(App);

App::App()
{
}

App::~App()
{
}

void App::initialize()
{
    if (getId() == 59) {
        // Boot the process scheduling the initial message as a self-message.
        EV << "only one message created, id :  " << getId() << endl;

        char msgname[20];
        int src = getIndex();  // our module index
        int size = getParentModule()->getVectorSize();
        int dst = intuniform(0, size-2);
        int slreq = intuniform(1, 3);

        EV << "vector size : "<< getParentModule()->getVectorSize() << endl;
        sprintf(msgname, "opt-%i-to-%i", src, dst);
        OpticalMsg *msg = new OpticalMsg(msgname);
        msg->setSrcAddr(src);
        msg->setDestAddr(dst);
        msg->setSlotReq(slreq);
        msg->setMsgState(LIGHTPATH_REQUEST);

        scheduleAt(0.0, msg);
    }
}

void App::handleMessage(cMessage *msg)
{
    send(msg, "out");
}

