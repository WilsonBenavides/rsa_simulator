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
public:
    DisplayManager();
    virtual ~DisplayManager();

protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override; // NOTE THE const MODIFIER!!!
    virtual void handleMessage(cMessage *msg) override;
    cCanvas *canvas;
    void drawSlotGrid(int slotSize, int linkSize, cFigure::Color color);
};

Define_Module(DisplayManager);

int DisplayManager::numInitStages() const
{
    return 3;
}

DisplayManager::DisplayManager()
{
}

DisplayManager::~DisplayManager()
{
}

void DisplayManager::initialize(int stage)
{
    if (stage == 2) {
        canvas = getParentModule()->getParentModule()->getCanvas();
        EV << " Display Manager, stage 2 " << endl;

//      #2A7997,blue   #A0E0A0,transparent   0xD1D1D1,gray  0x0086C6,dark_blue
        drawSlotGrid(10, 5, cFigure::Color("#A0E0A0"));
    }
}

void DisplayManager::handleMessage(cMessage *msg)
{
}

void DisplayManager::drawSlotGrid(int slotSize, int linkSize, cFigure::Color color)
{
    cRectangleFigure *linkGrid[slotSize];

    for (int lnk = 0; lnk < linkSize; lnk++) {
        for (int slt = 0; slt < slotSize; slt++) {
            int x = 800, y = 40;
            cRectangleFigure *rect = new cRectangleFigure();
            rect->setBounds(cFigure::Rectangle(x + (20 * slt), y + (20 * lnk + 10), 20, 20));
            rect->setCornerRadius(1);
            rect->setLineColor(cFigure::Color("#000000"));
            rect->setLineWidth(2);

            rect->setFilled(true);
            rect->setFillColor(color);

            if (lnk == 0 && (slt == 0 || slt == 1)) {
                rect->setFillColor(cFigure::Color("#000000"));
            }

//            linkGrid[slt] = rect;
            canvas->addFigure(rect);
        }
    }

//    linkGrid->push_back(rect);

}

