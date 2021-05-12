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
    std::string m_color;

    Slot(int index, int available, int right, int left, std::string color)
    {
        m_index = index;
        m_available = available;
        m_right = right;
        m_left = left;
        m_color = color;
    }
};

class Link
{
public:
    int m_id;
    bool m_availability;
    int m_slotSize;
    std::vector<Slot*> slots;

    Link(int id, bool availability, int slotSize)
    {
        m_id = id;
        m_availability = availability;
        m_slotSize = slotSize;
        std::string whiteColor("#ffffff");

        for (int i = 0; i < slotSize; i++) {
            Slot *sl = new Slot(i, 0, slotSize - i - 1, i, whiteColor);
            slots.push_back(sl);
        }
    }

    void PrintAvailableSlots()
    {
        for (Slot *sl : slots) {
            if (sl->m_available == 1) {
                EV << "id : " << m_id << " , ";
                EV << sl->m_index << '\t';
                EV << '\n';
            }
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
    void updateSlotGrid();
    std::vector<int> contiguousSpectrum(std::vector<Link*> lnk);
    std::vector<std::vector<int>> continuousSpectrum(std::vector<int> slots);
    Link* searchLink(int id);
    std::vector<int> algorithmFirstFit(std::vector<std::vector<int>> continuous, std::vector<Link*> lnk, int slrequest, cFigure::Color color);
    void printLinkMatrix();
    void clearLinkMatrix();
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
//    updateSlotGrid(numLinks, slotSize, 0);
    updateSlotGrid();
    clearLinkMatrix();
    numProcessed++;
    routeLinks.clear();

    char msgname[20];
    OpticalMsg *msgPath = check_and_cast<OpticalMsg*>(msg);
    int src = msgPath->getSrcAddr();
    int dst = msgPath->getDestAddr();
    int slreq = msgPath->getSlotReq();
    int id = msgPath->getId();
    int blue = msgPath->getColor() / 65536;
    int green = (msgPath->getColor() - blue * 65536) / 256;
    int red = msgPath->getColor() - blue * 65536 - green * 256;

    msgPath->setMsgState(LIGHTPATH_ASSIGNMENT);
    cFigure::Color col = cFigure::Color(red, green, blue);

    cTopology *topo = new cTopology("topo");
    std::vector<std::string> nedTypes;
    nedTypes.push_back(getParentModule()->getParentModule()->getSubmodule("node", 0)->getNedTypeName());
    topo->extractByNedTypeName(nedTypes);

    std::ifstream ifs("./node/TableRouting.csv");
    if (ifs.is_open()) {
        std::string node;
        std::string gate;
        std::string msgid;
        std::string lnkid;
        std::string color;
        std::string slot;

        while (ifs.good()) {
            std::getline(ifs, node, ',');
            std::getline(ifs, gate, ',');
            std::getline(ifs, msgid, ',');
            std::getline(ifs, lnkid, ',');
            std::getline(ifs, color, ',');
            std::getline(ifs, slot, '\n');

            if (node.length() != 0 && gate.length() != 0 && msgid.length() != 0 && lnkid.length() != 0 && color.length() != 0 && slot.length() != 0) {

                if (id == std::stoi(msgid)) {
                    cTopology::Node *tmpNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", std::stoi(node)));
                    routeLinks.push_back(searchLink(std::stoi(lnkid)));
                }
            }
        }
        ifs.close();
    }

    std::vector<int> temporal = contiguousSpectrum(routeLinks);
    std::vector<std::vector<int>> slts = continuousSpectrum(temporal);
    std::vector<int> slotFF = algorithmFirstFit(slts, routeLinks, slreq, col);

    if (slotFF.size() > 0) {

//        printLinkMatrix();

        ifs.open("./node/TableRouting.csv");
        if (ifs.is_open()) {
            std::string node;
            std::string gate;
            std::string msgid;
            std::string lnkid;
            std::string color;
            std::string slot;

            while (ifs.good()) {
                std::getline(ifs, node, ',');
                std::getline(ifs, gate, ',');
                std::getline(ifs, msgid, ',');
                std::getline(ifs, lnkid, ',');
                std::getline(ifs, color, ',');
                std::getline(ifs, slot, '\n');

                if (node.length() != 0 && gate.length() != 0 && msgid.length() != 0 && lnkid.length() != 0 && color.length() != 0 && slot.length() != 0) {

                    // pinta las rutas que se estan escritas en la tabla de enrutamiento
                    if (true) {
                        cTopology::Node *tmpNode = topo->getNodeFor(getParentModule()->getParentModule()->getSubmodule("node", std::stoi(node)));
                        cDisplayString &dispStr = tmpNode->getLinkOut(std::stoi(gate))->getLocalGate()->getDisplayString();
                        char tcol[30];
                        sprintf(tcol, "ls=%s,5", color.c_str());
                        dispStr.parse(tcol);
//                        EV << "lnk id : " << lnkid << " 1 " << " slot : " << slotFF << endl;
                        for (Link *tlnk : linkMatrix) {
                            for (Slot *tsl : tlnk->slots) {
//                                if (tlnk->m_id == std::stoi(lnkid) && tsl->m_index == slotFF) {
                                if (tlnk->m_id == std::stoi(lnkid)) {
//                                    tsl->m_available = 1;
//                                    EV << "lnk id : " << tlnk->m_id << "  slot : " << tsl->m_index << endl;
//                                    drawSingleSlotOnGrid(tlnk->m_id, tsl->m_index, cFigure::Color(color.c_str()) );
                                }
                            }
                        }
                    }
                }
            }
            ifs.close();
        }
//        printLinkMatrix();
//        updateSlotGrid();

        cModule *srcNode = getParentModule()->getParentModule()->getSubmodule("node", src)->getSubmodule("bvwxc");
        sendDirect(msgPath, srcNode, "directIn");
//        delete msgPath;
    }
    else {
        numLost++;
        getParentModule()->bubble("lost packet");
        int pkid = msgPath->getId();
        std::ifstream ifs("./node/TableRouting.csv");
        std::ofstream tmp;
        if (ifs.is_open()) {
            tmp.open("./node/lost.csv");
            //            tmp.open("./node/tmp.csv", std::ios_base::app);
            std::string node;
            std::string gate;
            std::string msgid;
            std::string lnkid;
            std::string color;
            std::string slot;

            while (ifs.good()) {
                std::getline(ifs, node, ',');
                std::getline(ifs, gate, ',');
                std::getline(ifs, msgid, ',');
                std::getline(ifs, lnkid, ',');
                std::getline(ifs, color, ',');
                std::getline(ifs, slot, '\n');

                if (node.length() != 0 && gate.length() != 0 && msgid.length() != 0 && lnkid.length() != 0 && color.length() != 0 && slot.length() != 0) {
                    if (pkid != std::stoi(msgid)) {
                        tmp << node << "," << gate << "," << msgid << "," << lnkid << "," << color << "," << slot << endl;
                    }
                }
            }
            tmp.close();
            ifs.close();
        }
        std::remove("./node/TableRouting.csv");
        std::rename("./node/lost.csv", "./node/TableRouting.csv");
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

void SpectrumManager::updateSlotGrid()
{
//    for (int lnk = 0; lnk < linkSize; lnk++) {
//        for (int slt = 0; slt < slotSize; slt++) {
//            char name[10];
//            sprintf(name, "%d,%d", lnk, slt);
//            cRectangleFigure *rct = static_cast<cRectangleFigure*>(canvas->getFigure(name));
////            if (std::string("#ffffff") != rct->getFillColor().str()) {
//            rct->setFillColor("#ffffff");
////                rct->setFillOpacity(opacity);
//            rct->setLineWidth(1);
////            }
//        }
//    }

    for (Link *lnk : linkMatrix) {
        for (Slot *sl : lnk->slots) {
            char name[10];
            sprintf(name, "%d,%d", lnk->m_id, sl->m_index);
            cRectangleFigure *rct = static_cast<cRectangleFigure*>(canvas->getFigure(name));
            rct->setFillColor("#ffffff");
            rct->setLineWidth(1);
        }
    }
}

std::vector<int> SpectrumManager::contiguousSpectrum(std::vector<Link*> lnk)
{
    int size = lnk.back()->m_slotSize;
    std::vector<int> t(size, 0);
    for (Link *l : lnk) {
        for (int i = 0; i < size; i++) {

            t[i] = (t[i] + l->slots[i]->m_available) >= 1 ? 1 : 0;
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

std::vector<int> SpectrumManager::algorithmFirstFit(std::vector<std::vector<int>> continuous, std::vector<Link*> lnk, int slrequest, cFigure::Color color)
{
    std::vector<int> tmp;
    int flag = 0;
    int slot = -1;
    for (int i = 0; i < continuous.size(); i++) {
        if (continuous[i].size() >= slrequest && continuous[i][0] != BUSY) {
            flag++;
            for (Link *l : lnk) {
                for (int j = continuous[i][0]; j < continuous[i][0] + slrequest; j++) {
                    l->slots[j]->m_available = 1;
//                    EV << "first fit : index " << l->slots[j]->m_index << " value : " << l->slots[j]->m_available << endl;
                    drawSingleSlotOnGrid(l->m_id, j, color);
                    slot = j;
                    tmp.push_back(j);
                }
            }
        }
        if (flag == 1) {
            return tmp;
//            break;
        }
    }
    if (flag == 0) {
        return tmp; //enlace congestionado
    }
    return tmp;
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

void SpectrumManager::printLinkMatrix()
{
    EV << "link Matrix : " << endl;
    for (Link *ln : linkMatrix) {
        ln->PrintAvailableSlots();
//        EV << endl;
    }
}
void SpectrumManager::clearLinkMatrix()
{
//    EV << "clear Matrix : " << endl;
    for (Link *ln : linkMatrix) {
        for (Slot *sl : ln->slots) {
            sl->m_available = 0;
        }
    }
}
