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
        EV << " Display Manager, stage 2 " << endl;
        cCanvas *canvas = getParentModule()->getParentModule()->getCanvas();

//        cLineFigure *line = new cLineFigure("line");
//        line->setStart(cFigure::Point(0, 0));
//        line->setEnd(cFigure::Point(100, 50));
//        line->setLineWidth(2);
//        line->setEndArrowhead(cFigure::ARROW_BARBED);
//
//        cArcFigure *arc = new cArcFigure("arc");
//        arc->setBounds(cFigure::Rectangle(10,10,100,100));
//        arc->setStartAngle(0);
//        arc->setEndAngle(M_PI/2);
//        arc->setLineColor(cFigure::BLUE);
//        arc->setEndArrowhead(cFigure::ARROW_BARBED);

        cRectangleFigure *rect = new cRectangleFigure("rect");
        rect->setBounds(cFigure::Rectangle(100, 100, 160, 100));
        rect->setCornerRadius(5);
        rect->setFilled(true);
        rect->setFillColor(cFigure::GOOD_LIGHT_COLORS[0]);

        canvas->addFigure(rect);
//        canvas->addFigure(arc);
    }
}

void DisplayManager::handleMessage(cMessage *msg)
{
}

