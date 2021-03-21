//
//
#include "TopologyManager.h"

Define_Module(TopologyManager);

int TopologyManager::numInitStages() const
{
    return 3;
}

TopologyManager::TopologyManager()
{
    topo.extractByModulePath(cStringTokenizer("**.ann_arbor **.boulder **.champaign").asVector());
    EV << "num of nodes : " << topo.getNumNodes() << endl;
}

TopologyManager::~TopologyManager()
{
}

void TopologyManager::initialize(int stage)
{
    if (stage == 1) {
        EV << " Topology Manager, stage 1 " << endl;
    }
}

void TopologyManager::handleMessage(cMessage *msg)
{
        EV << " Topology Manager handleMessage  " << endl;
}

int TopologyManager::getNumNodes()
{
    return topo.getNumNodes();
}

