//
//
#include <vector>
#include <string>
#include <omnetpp.h>
#include "Utils.h"

using namespace omnetpp;

/**
 *
 */
class DisplayManager : public cSimpleModule
{

public:
    DisplayManager() = default;
    virtual ~DisplayManager();

protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override; // NOTE THE const MODIFIER!!!
    virtual void handleMessage(cMessage *msg) override;

};

Define_Module(DisplayManager);

int DisplayManager::numInitStages() const
{
    return 3;
}

DisplayManager::~DisplayManager()
{
}

void DisplayManager::initialize(int stage)
{


    if (stage == 1) {
//        EV << " Display Manager, stage 1 " << endl;

//      #2A7997,blue   #A0E0A0,transparent   #D1D1D1,gray  #0086C6,dark_blue
//        drawSlotGrid(linkSize, slotSize, cFigure::Color("#ffffff"));

//        drawSlotsOnGrid(0, 0, 4, generateRandomColor());

//        drawSlotsOnGrid(3, 3, 5, generateRandomColor());
//        drawSlotsOnGrid(0, 2, 4, generateRandomColor());
//        drawSlotsOnGrid(0, 7, 4, generateRandomColor());
//        drawSlotsOnGrid(0, 11, 4, generateRandomColor());
//        drawSlotsOnGrid(0, 15, 4, generateRandomColor());
//
//        drawSlotsOnGrid(1, 2, 4, generateRandomColor());
//        drawSlotsOnGrid(1, 6, 1, generateRandomColor());
//        drawSlotsOnGrid(1, 7, 5, generateRandomColor());
    }
}

void DisplayManager::handleMessage(cMessage *msg)
{
}




