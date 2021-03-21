//
//
#include <vector>
#include <string>
#include <omnetpp.h>

#define STR_LEN 7

using namespace omnetpp;

/**
 * Generates traffic for the network.
 */
class DisplayManager : public cSimpleModule
{
    cCanvas *canvas;
    int slotSize = 10, linkSize = 21;
public:
    DisplayManager() = default;
    virtual ~DisplayManager();

protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override; // NOTE THE const MODIFIER!!!
    virtual void handleMessage(cMessage *msg) override;
    void drawSlotGrid(int slotSize, int linkSize, cFigure::Color color);
    void drawSlotsOnGrid(int lnkPos, int sltPos, int numSlots, cFigure::Color color);
    cFigure::Color generateRandomColor();
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
    canvas = getParentModule()->getParentModule()->getCanvas();

    if (stage == 1) {
        EV << " Display Manager, stage 1 " << endl;

//      #2A7997,blue   #A0E0A0,transparent   #D1D1D1,gray  #0086C6,dark_blue
        drawSlotGrid(linkSize, slotSize, cFigure::Color("#ffffff"));
        drawSlotsOnGrid(0, 8, 2, generateRandomColor());
//        drawSlotsOnGrid(0, 3, 4, generateRandomColor());
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

void DisplayManager::drawSlotGrid(int linkSize, int slotSize, cFigure::Color color)
{
    for (int lnk = 0; lnk < linkSize; lnk++) {
        for (int slt = 0; slt < slotSize; slt++) {
            int x = 800, y = 20;
            char name[15];
            sprintf(name, "%d,%d", lnk, slt);  //(link, slot) format
            EV << name << endl;
            cRectangleFigure *rect = new cRectangleFigure(name);
            rect->setBounds(cFigure::Rectangle(x + (20 * slt), y + (25 * lnk), 20, 20));
            rect->setCornerRadius(1);
            rect->setLineColor(cFigure::Color("#000000"));
            rect->setLineWidth(2);
            rect->setFilled(true);
            rect->setFillColor(color);

            canvas->addFigure(rect);
        }
    }

}

void DisplayManager::drawSlotsOnGrid(int lnkPos, int sltPos, int numSlots, cFigure::Color color)
{
    for (int i = 0; i < numSlots; i++) {
        char name[10];
        sprintf(name, "%d,%d", lnkPos, sltPos + i);
        cRectangleFigure *rct = static_cast<cRectangleFigure*>(canvas->getFigure(name));
        rct->setFillColor(color);
    }
}

cFigure::Color DisplayManager::generateRandomColor()
{
    unsigned char str[STR_LEN + 1] = { 0 };
    const char *hex_digits = "0123456789ABCDEF";
    for (int i = 0; i < STR_LEN; i++) {
        str[i] = hex_digits[(rand() % 16)];
    }
    str[0] = '#';
    return cFigure::Color((char*) str);
}
