//
//

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

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    cFigure::Color generateRandomColor();
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
        int dst = intuniform(0, size - 2);
        int slreq = intuniform(1, 3);

        sprintf(msgname, "opt-%i-to-%i-numSlot-%i", src, dst, slreq);
        OpticalMsg *msg = new OpticalMsg(msgname);
        msg->setSrcAddr(src);
        msg->setDestAddr(dst);
        msg->setSlotReq(slreq);
        msg->setMsgState(LIGHTPATH_REQUEST);
        msg->setRed(intuniform(0, 255));
        msg->setGreen(intuniform(0, 255));
        msg->setBlue(intuniform(0, 255));

        scheduleAt(0.0, msg);
    }
}

void App::handleMessage(cMessage *msg)
{
    send(msg, "out");
}



