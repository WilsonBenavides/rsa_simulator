//
//
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <iomanip>
#include <omnetpp.h>
#include "OpticalMsg_m.h"
#include "Utils.h"

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
    long numProcessed;
    long numLost;
    const char *assignmentAlgorithm;
    std::vector<Link*> routeLinks;
    std::vector<Link*> linkMatrix;

public:
    SpectrumManager() = default;
    virtual ~SpectrumManager();

protected:

    virtual void initialize(int stage) override;
    virtual int numInitStages() const override; // NOTE THE const MODIFIER!!!
    virtual void finish() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;
    void drawSlotGrid(int linkSize, int slotSize, cFigure::Color color);
    void drawSlotsOnGrid(int lnkPos, int sltPos, int numSlots, cFigure::Color color);
    void drawSingleSlotOnGrid(int lnkPos, int sltPos, cFigure::Color color);
    void updateSlotGrid(int linkSize, int slotSize, double opacity);
    std::vector<int> contiguousSpectrum(std::vector<Link*> lnk);
    std::vector<std::vector<int>> continuousSpectrum(std::vector<int> slots);
    Link* searchLink(int id);
    bool algorithmFirstFit(std::vector<std::vector<int>> continuous, std::vector<Link*> lnk, int slrequest, cFigure::Color color);
};

Define_Module(SpectrumManager);

int SpectrumManager::numInitStages() const
{
    return 2;
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

    numProcessed = 0;
    numLost = 0;
    WATCH(numProcessed);
    WATCH(numLost);

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
                Link *l = new Link(linkId, true, slotSize);
                linkMatrix.push_back(l);
                numLinks++;
            }
        }
        drawSlotGrid(numLinks, slotSize, cFigure::Color("#ffffff"));
        delete topo;
    }
}

void SpectrumManager::handleMessage(cMessage *msg)
{
    updateSlotGrid(numLinks, slotSize, 0);
    numProcessed++;

    cTopology *topo = new cTopology("topo");
    std::vector<std::string> nedTypes;
    nedTypes.push_back(getParentModule()->getParentModule()->getSubmodule("node", 0)->getNedTypeName());
    topo->extractByNedTypeName(nedTypes);

    std::string line;
    std::ifstream ifs("./node/TableRouting.csv");
    if (ifs.is_open()) {
        while (std::getline(ifs, line)) {
            //                EV << line << endl;
            int index = line.find(',');
            int node = std::stoi(line.substr(0, index++));
            int gate = std::stoi(line.substr(index, line.size()));

            cTopology::Node *tmpNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", node));
            cDisplayString &dispStr = tmpNode->getLinkOut(gate)->getLocalGate()->getDisplayString();
//            dispStr.parse("ls=#000000,5");
        }
        ifs.close();
    }

    char msgname[20];
    OpticalMsgPath *msgPath = check_and_cast<OpticalMsgPath*>(msg);
    int src = msgPath->getSrcAddr();
    int dst = msgPath->getDestAddr();
    int slreq = msgPath->getSlotReq();
//    int state = msgPath->getMsgState();
    int blue = msgPath->getColor() / 65536;
    int green = (msgPath->getColor() - blue * 65536) / 256;
    int red = msgPath->getColor() - blue * 65536 - green * 256;
    cFigure::Color col = cFigure::Color(red, green, blue);

    sprintf(msgname, "%i-%i-ns-%i", src, dst, slreq);
    OpticalMsg *msgNode = new OpticalMsg(msgname);
    msgNode->setSrcAddr(src);
    msgNode->setDestAddr(dst);
    msgNode->setSlotReq(slreq);
    msgNode->setMsgState(LIGHTPATH_ASSIGNMENT);
//    EV << "spec byte length" << msgPath->getByteLength() << endl;
    msgNode->setByteLength(msgPath->getByteLength());

    routeLinks.clear();

    for (int id = 0; id < msgPath->getOpticalPathArraySize(); id++) {
        int lnkId = msgPath->getOpticalPath(id);
        routeLinks.push_back(searchLink(lnkId));
    }

    std::vector<int> temporal = contiguousSpectrum(routeLinks);

    std::vector<std::vector<int>> slts = continuousSpectrum(temporal);

    bool linkState = algorithmFirstFit(slts, routeLinks, slreq, col);

    if (linkState) {
        cModule *srcNode = getParentModule()->getParentModule()->getSubmodule("node", src)->getSubmodule("bvwxc");
        sendDirect(msgNode, srcNode, "directIn");
        delete msgPath;
    }
    else {
        numLost++;
        getParentModule()->bubble("lost packet");
        delete msgPath;
    }
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

            if (lnk == 0) {
                char text[15];
                sprintf(text, " %2d", slt + 1);  //(link, slot) format
                cTextFigure *txtFig = new cTextFigure();
                txtFig->setText(text);
                txtFig->setPosition(cFigure::Point(x + (width * slt), y - 20 + ((height + 2) * lnk)));
                canvas->addFigure(txtFig);
            }

            if (lnk == 0 && slt == 0) {
                char text2[] = "Slots:";
                cTextFigure *txtFig2 = new cTextFigure();
                txtFig2->setText(text2);
                txtFig2->setPosition(cFigure::Point(x - 40, y - 20 + ((height + 2) * lnk)));
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
    rct->setLineWidth(4);
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

void SpectrumManager::updateSlotGrid(int linkSize, int slotSize, double opacity)
{
    for (int lnk = 0; lnk < linkSize; lnk++) {
        for (int slt = 0; slt < slotSize; slt++) {
            char name[10];
            sprintf(name, "%d,%d", lnk, slt);
            cRectangleFigure *rct = static_cast<cRectangleFigure*>(canvas->getFigure(name));
            if (std::string("#ffffff") != rct->getFillColor().str()) {
                rct->setFillOpacity(opacity);
                rct->setLineWidth(1);
            }
        }
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

bool SpectrumManager::algorithmFirstFit(std::vector<std::vector<int>> continuous, std::vector<Link*> lnk, int slrequest, cFigure::Color color)
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
        if (flag == 1) {
            return true;
//            break;
        }
    }
    if (flag == 0) {
        return false;
    }
    return false;
}
Link* SpectrumManager::searchLink(int id)
{
    for (Link *ln : linkMatrix) {
        if (id == ln->m_id) {
            return ln;
        }
    }
    return nullptr;
}

void SpectrumManager::finish()
{
}

void SpectrumManager::refreshDisplay() const
{
    char buf[40];
    sprintf(buf, "p: %ld  l: %ld", numProcessed, numLost);
    getParentModule()->getDisplayString().setTagArg("t", 0, buf);
}
