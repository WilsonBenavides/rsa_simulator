//
//

#include <vector>
#include <string>
#include <omnetpp.h>

using namespace omnetpp;

/**
 * Generates traffic for the network.
 */
class DisplayManager : public cSimpleModule
{
    cCanvas *canvas;
    int slotSize = 20, linkSize = 21;
public:
    DisplayManager() = default;
    virtual ~DisplayManager();

protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override; // NOTE THE const MODIFIER!!!
    virtual void handleMessage(cMessage *msg) override;
    void drawSlotGrid(int slotSize, int linkSize, cFigure::Color color);
    void drawSlotsOnGrid(int xpos, int ypos, int numSlots, cFigure::Color color);

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

    if (stage == 2) {
        EV << " Display Manager, stage 2 " << endl;

//      #2A7997,blue   #A0E0A0,transparent   #D1D1D1,gray  #0086C6,dark_blue
        drawSlotGrid(slotSize, linkSize, cFigure::Color("#A0E0A0"));
        drawSlotsOnGrid(1, 0, 2, cFigure::Color("#21ab73"));
        drawSlotsOnGrid(3, 0, 4, cFigure::Color("#5f1350"));
        drawSlotsOnGrid(2, 1, 4, cFigure::Color("#cb5184"));
        drawSlotsOnGrid(6, 1, 1, cFigure::Color("#7d4ea3"));
    }
}

void DisplayManager::handleMessage(cMessage *msg)
{
}

void DisplayManager::drawSlotGrid(int slotSize, int linkSize, cFigure::Color color)
{
    for (int lnk = 0; lnk < linkSize; lnk++) {
        for (int slt = 0; slt < slotSize; slt++) {
            int x = 800, y = 20;
            cRectangleFigure *rect = new cRectangleFigure();
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

void DisplayManager::drawSlotsOnGrid(int xpos, int ypos, int numSlots, cFigure::Color color)
{
    int pos = xpos + slotSize * (ypos) + 1;
    for (int i = pos; i < pos + numSlots; i++) {
        cRectangleFigure *rct = static_cast<cRectangleFigure*>(canvas->getFigure(i));
        rct->setFillColor(color);
    }
}
