//
//
#include <vector>
#include <string>
#include <omnetpp.h>
#include "Utils.h"
#include "OpticalMsg_m.h"

#define BUSY -10

using namespace omnetpp;

class Slot
{
public:
    int m_index;
    int m_available;  //1 busy, 0 available
    int m_right;
    int m_left;
    double m_time;

    Slot(int index, int available, int right, int left, double time)
    {
        m_index = index;
        m_available = available;
        m_right = right;
        m_left = left;
        m_time = time;
    }
};

class Link
{
public:
    int m_id;
    bool m_availability;
    int m_slotSize;
    std::vector<Slot> slots;

    Link(int id, bool availability, int slotSize)
    {
        m_id = id;
        m_availability = availability;
        m_slotSize = slotSize;

        for (int i = 0; i < slotSize; i++) {
            Slot sl(i, 0, slotSize - i - 1, i, 0.0);
            slots.push_back(sl);
        }
    }

    void PrintAvailableSlots()
    {
        EV << "id : " << m_id << endl;
        for (Slot sl : slots) {
            EV << "slots : " << sl.m_available << endl;
        }
    }
};

/**
 *
 */
class SpectrumManager : public cSimpleModule
{
    cCanvas *canvas = new cCanvas();
    int slotSize = 10, linkSize = 21;
    int numLinks;
    double slotBandwidth;
    double channelBandwidth;
    const char *assignmentAlgorithm;
    std::vector<Link*> routeLinks;
    std::vector<Link*> linkMatrix;

public:
    SpectrumManager() = default;
    virtual ~SpectrumManager();

    struct SlotItem
    {
        int index;
        int slt_right;
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
        std::vector<int> slots_availability;
        std::vector<int> slots_right;
        std::vector<SlotBox> vec_box;
    };
    std::vector<LinkData> LinkTable;

protected:

    virtual void initialize(int stage) override;
    virtual int numInitStages() const override; // NOTE THE const MODIFIER!!!
    virtual void finish() override;
    virtual void handleMessage(cMessage *msg) override;
    void drawSlotGrid(int slotSize, int linkSize, cFigure::Color color);
    void drawSlotsOnGrid(int lnkPos, int sltPos, int numSlots, cFigure::Color color);
    void drawSingleSlotOnGrid(int lnkPos, int sltPos, cFigure::Color color);
    std::vector<int> contiguousSpectrum(std::vector<Link*> lnk);
    std::vector<std::vector<int>> continuousSpectrum(std::vector<int> slots);
    Link* searchLink(int id);
    void algorithmFirstFit(std::vector<std::vector<int>> continuous, std::vector<Link*> lnk, int slrequest, cFigure::Color color);
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
    assignmentAlgorithm = par("assignmentAlgorithm");
    slotSize = (channelBandwidth / slotBandwidth);
    canvas = getParentModule()->getParentModule()->getCanvas(); // toplevel canvas

    std::vector<SlotBox> vec_box;
    SlotBox box;
    std::vector<int> sl_rg(slotSize, 0);
    std::vector<int> sl_av(slotSize, 0);

    if (stage == 1) {
        for (int i = 0; i < sl_rg.size(); i++) {
            int sl_right = static_cast<int>(sl_rg.size() - i - 1);
            SlotItem si = { i, sl_right };
            sl_rg.at(i) = sl_right;
            box.slot_box.push_back(si);
        }
        vec_box.push_back(box);

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
                Link *l = new Link(linkId, true, slotSize);
                linkMatrix.push_back(l);

                LinkData data = { linkId, src, dst, gate, true, slotSize, 0, sl_av, sl_rg, vec_box };
                LinkTable.push_back(data);
                numLinks++;
            }
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

    sprintf(msgname, "%i-%i-ns-%i", src, dst, slreq);
    OpticalMsg *msgNode = new OpticalMsg(msgname);
    msgNode->setSrcAddr(src);
    msgNode->setDestAddr(dst);
    msgNode->setSlotReq(slreq);
    msgNode->setMsgState(LIGHTPATH_ASSIGNMENT);

    routeLinks.clear();

    for (int id = 0; id < msgPath->getOpticalPathArraySize(); id++) {
        int lnkId = msgPath->getOpticalPath(id);
        routeLinks.push_back(searchLink(lnkId));
    }

    std::vector<int> temporal = contiguousSpectrum(routeLinks);

    std::vector<std::vector<int>> slts = continuousSpectrum(temporal);

    algorithmFirstFit(slts, routeLinks, slreq, col);


    cModule *srcNode = getParentModule()->getParentModule()->getSubmodule("node", src)->getSubmodule("bvwxc");
    sendDirect(msgNode, srcNode, "directIn");
    delete msgPath;
}

void SpectrumManager::drawSlotGrid(int linkSize, int slotSize, cFigure::Color color)
{
    int width = 12 + 9, height = 12;
    int x = 840, y = 0;
    for (int lnk = 0; lnk < linkSize; lnk++) {
        char txt[15];
        sprintf(txt, "Link %2d", lnk + 1);  //(link, slot) format
        cTextFigure *textFigure = new cTextFigure();
        textFigure->setText(txt);
        textFigure->setPosition(cFigure::Point(x - 45, y + ((height + 2) * lnk)));
        canvas->addFigure(textFigure);

        for (int slt = 0; slt < slotSize; slt++) {
            char name[15];
            sprintf(name, "%d,%d", lnk, slt);  //(link, slot) format
            cRectangleFigure *rect = new cRectangleFigure(name);

            if (lnk == linkSize - 1) {
                char text[15];
                sprintf(text, " %2d", slt + 1);  //(link, slot) format
                cTextFigure *txtFig = new cTextFigure();
                txtFig->setText(text);
                txtFig->setPosition(cFigure::Point(x + (width * slt), y + 15 + ((height + 2) * lnk)));
                canvas->addFigure(txtFig);
            }

            if (lnk == linkSize - 1 && slt == 0) {
                char text2[] = "Slots:";
                cTextFigure *txtFig2 = new cTextFigure();
                txtFig2->setText(text2);
                txtFig2->setPosition(cFigure::Point(x - 40, y + 15 + ((height + 2) * lnk)));
                canvas->addFigure(txtFig2);
            }

            rect->setBounds(cFigure::Rectangle(x + (width * slt), y + ((height + 2) * lnk), width, height));
            rect->setCornerRadius(1);
            rect->setLineColor(cFigure::Color("#000000"));
            rect->setLineWidth(1);
            rect->setFilled(true);
            rect->setFillColor(color);
            canvas->addFigure(rect);
        }
    }

}

void SpectrumManager::drawSingleSlotOnGrid(int lnkPos, int sltPos, cFigure::Color color)
{
    char name[10];
    sprintf(name, "%d,%d", lnkPos, sltPos);
    cRectangleFigure *rct = static_cast<cRectangleFigure*>(canvas->getFigure(name));
    rct->setFillColor(color);
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

std::vector<int> SpectrumManager::contiguousSpectrum(std::vector<Link*> lnk)
{
    int size = lnk.back()->m_slotSize;
    std::vector<int> t(size, 0);
    for (Link *l : lnk) {
        for (int i = 0; i < size; i++) {
            t[i] = (t[i] + l->slots[i].m_available) >= 1 ? 1 : 0;
        }
    }
    std::vector<int> c(size, 0);
    for (int i = 0; i < size; i++) {
        c[i] = t[i] == 1 ? BUSY : i;
    }

    return c;
}

std::vector<std::vector<int>> SpectrumManager::continuousSpectrum(std::vector<int> slots)
{
    std::vector<std::vector<int>> result;
    result.resize(1);
    result.back().push_back(slots[0]);
    for (int i = 1; i < slots.size(); i++) {
        int i_bef = slots[i];
        int i_lat = slots[i - 1] + 1;
        if (i_bef != i_lat) {
            result.push_back(std::vector<int>());
        }
        result.back().push_back(i_bef);
    }

    return result;
}

void SpectrumManager::algorithmFirstFit(std::vector<std::vector<int>> continuous, std::vector<Link*> lnk, int slrequest, cFigure::Color color)
{
    int flag = 0;
    for (int i = 0; i < continuous.size(); i++) {
        if (continuous[i].size() >= slrequest && continuous[i][0] != BUSY) {
            flag++;
            for (Link *l : lnk) {
                for (int j = continuous[i][0]; j < continuous[i][0] + slrequest; j++) {
                    l->slots[j].m_available = 1;
                    drawSingleSlotOnGrid(l->m_id, j, color);
                }
            }
        }
        if (flag == 1)
            break;
    }
}
Link* SpectrumManager::searchLink(int id)
{
    for (Link *ln : linkMatrix) {
        if (id == ln->m_id) {
            return ln;
        }
    }
}

void SpectrumManager::finish()
{
}

