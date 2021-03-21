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


