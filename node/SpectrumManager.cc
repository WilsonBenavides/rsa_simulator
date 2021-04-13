//
//
#include <vector>
#include <string>
#include <omnetpp.h>
#include "OpticalMsg_m.h"
#include "Utils.h"

using namespace omnetpp;

/**
 *
 */
class SpectrumManager : public cSimpleModule
{
    cCanvas *canvas;
    int slotSize = 10, linkSize = 21;
    int numLinks;
    double slotBandwidth;
    double channelBandwidth;
public:
    SpectrumManager() = default;
    virtual ~SpectrumManager();
    struct SlotItem
    {
        int index;
        int value;
    };
    struct SlotBox
    {
        std::vector<SlotItem> slot_box;
    };
    struct LinkData
    {
        int id;
        int source;
        int destination;
        int gate;
        bool availability;
        int available_slots;
        int used_slots;
        std::vector<int> slots;
        std::vector<SlotBox> vec_box;
    };
    std::vector<LinkData> LinkTable;

protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override; // NOTE THE const MODIFIER!!!
    virtual void handleMessage(cMessage *msg) override;
    void drawSlotGrid(int slotSize, int linkSize, cFigure::Color color);
    void drawSlotsOnGrid(int lnkPos, int sltPos, int numSlots, cFigure::Color color);
};

Define_Module(SpectrumManager);

int SpectrumManager::numInitStages() const
{
    return 3;
}

SpectrumManager::~SpectrumManager()
{
}

void SpectrumManager::initialize(int stage)
{
    slotBandwidth = par("slotBandwidth");
    channelBandwidth = par("channelBandwidth");
    canvas = getParentModule()->getParentModule()->getCanvas();

    slotSize = (channelBandwidth / slotBandwidth);
    std::vector<SlotBox> vec_box;
    SlotBox box;
    std::vector<int> tmp(slotSize, 0);

    for (int i = 0; i < tmp.size(); i++) {
        SlotItem si = { i, tmp.at(i) };
        box.slot_box.push_back(si);
    }
    vec_box.push_back(box);

    for (SlotItem sb : vec_box.at(0).slot_box) {
//        EV << " index : " << sb.index << "  :  value  : " << sb.value << endl;
    }

    if (stage == 1) {

        cTopology *topo = new cTopology("topo");
        topo->extractByModulePath(cStringTokenizer("**.node[*]").asVector());
        numLinks = 0;
        for (int i = 0; i < topo->getNumNodes(); i++) {
            cTopology::Node *srcNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", i));
            int numOutLinks = srcNode->getNumOutLinks();
            for (int j = 0; j < numOutLinks; j++) {
                int src = srcNode->getModule()->par("address");
                int dst = topo->getNode(i)->getLinkOut(j)->getRemoteNode()->getModule()->par("address");
                int gate = srcNode->getLinkOut(j)->getLocalGate()->getIndex();
                int linkId = srcNode->getLinkOut(j)->getLocalGate()->getConnectionId();
                LinkData data = { linkId, src, dst, gate, true, slotSize, 0, tmp, vec_box };
                LinkTable.push_back(data);
                numLinks++;
            }
        }

        for (int it : tmp) {
            EV << " : " << it;
        }
        drawSlotGrid(numLinks, slotSize, cFigure::Color("#ffffff"));
        delete topo;
    }
}

void SpectrumManager::handleMessage(cMessage *msg)
{
    char msgname[20];
    OpticalMsgPath *msgPath = check_and_cast<OpticalMsgPath*>(msg);
    int src = msgPath->getSrcAddr();
    int dst = msgPath->getDestAddr();
    int slreq = msgPath->getSlotReq();
    int state = msgPath->getMsgState();
    int red = msgPath->getRed();
    int green = msgPath->getGreen();
    int blue = msgPath->getBlue();
    cFigure::Color col = cFigure::Color(red, green, blue);

    sprintf(msgname, "opt-%i-to-%i-ns-%i", src, dst, slreq);
    OpticalMsg *msgNode = new OpticalMsg(msgname);
    msgNode->setSrcAddr(src);
    msgNode->setDestAddr(dst);
    msgNode->setSlotReq(slreq);
    msgNode->setMsgState(LIGHTPATH_ASSIGNMENT);


    for (int id = 0; id < msgPath->getOpticalPathArraySize(); id++) {
        std::vector<int> usedId;
        int lnkId = msgPath->getOpticalPath(id);
        for (LinkData lnk : LinkTable) {
            if (lnk.id == lnkId) {
                EV << " slots :  ";
                lnk.slots.at(5) = 1;
                lnk.slots.at(6) = 1;
                lnk.slots.at(7) = 1;

                for (int tile : lnk.slots) {
                    EV << tile;
                }

                for (int i = 0; i < lnk.slots.size(); i++) {
                    if (lnk.slots.at(i) == 0) {
                        EV << " i:  " << i << " value: " << lnk.slots.at(i) << endl;
                    } else if(usedId.size() == 0){
                        EV << " i__:  " << i << " value: " << lnk.slots.at(i) << endl;
                        usedId.push_back(i);

                    }
                }
            }
        }
//        drawSlotsOnGrid(gateId, 0, slreq, cFigure::Color(col));
//        EV << "gateId : " << gateId << endl;
    }

    cModule *srcNode = getParentModule()->getParentModule()->getSubmodule("node", src)->getSubmodule("bvwxc");
    delete msgPath;
    sendDirect(msgNode, srcNode, "directIn");

//
//    for (LinkData tmp : LinkTable) {
//        //        EV << "index : " << tmp.index << " source : " << tmp.source << " destination : " << tmp.destination << " gate : " << tmp.gate << endl;
//
//    }
}

void SpectrumManager::drawSlotGrid(int linkSize, int slotSize, cFigure::Color color)
{
    for (int lnk = 0; lnk < linkSize; lnk++) {
        for (int slt = 0; slt < slotSize; slt++) {
            int x = 800, y = 0;
            char name[15];
            sprintf(name, "%d,%d", lnk, slt);  //(link, slot) format
//            EV << name << endl;
            cRectangleFigure *rect = new cRectangleFigure(name);
            rect->setBounds(cFigure::Rectangle(x + (12 * slt), y + (14 * lnk), 12, 12));
            rect->setCornerRadius(1);
            rect->setLineColor(cFigure::Color("#000000"));
            rect->setLineWidth(1);
            rect->setFilled(true);
            rect->setFillColor(color);

            canvas->addFigure(rect);
        }
    }

}

void SpectrumManager::drawSlotsOnGrid(int lnkPos, int sltPos, int numSlots, cFigure::Color color)
{
    for (int i = 0; i < numSlots; i++) {
        char name[10];
        sprintf(name, "%d,%d", lnkPos, sltPos + i);
        cRectangleFigure *rct = static_cast<cRectangleFigure*>(canvas->getFigure(name));
        rct->setFillColor(color);
    }
}

