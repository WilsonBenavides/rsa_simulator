/*
 * TopologyManager.h
 *
 *  Created on: 21/03/2021
 *      Author: Wilson
 */

#ifndef NODE_TOPOLOGYMANAGER_H_
#define NODE_TOPOLOGYMANAGER_H_



#include <vector>
#include <string>
#include <omnetpp.h>

using namespace omnetpp;

/**
 * Generates traffic for the network.
 */
class TopologyManager : public cSimpleModule
{
public:
    TopologyManager();
    virtual ~TopologyManager();

protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override; // NOTE THE const MODIFIER!!!
    virtual void handleMessage(cMessage *msg) override;

private:

};


#endif /* NODE_TOPOLOGYMANAGER_H_ */
