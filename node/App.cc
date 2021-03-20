//
//

#include <vector>
#include <string>
#include <omnetpp.h>
#include "Packet_m.h"

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
    if (getId() == 63) {
        // Boot the process scheduling the initial message as a self-message.
        EV << "only one message created, id :  " << getId() << endl;
        char msgname[20];
        sprintf(msgname, "msg-%i", getId());
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
}

void App::handleMessage(cMessage *msg)
{
    send(msg, "out");
}

