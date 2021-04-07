//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

/**
 * Point-to-point interface module. While one frame is transmitted,
 * additional frames get queued up; see NED file for more info.
 */
class BVT : public cSimpleModule
{
  private:

  public:
    BVT();
    virtual ~BVT();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;
    virtual void startTransmitting(cMessage *msg);
};

Define_Module(BVT);

BVT::BVT()
{
}

BVT::~BVT()
{
}

void BVT::initialize()
{
}

void BVT::startTransmitting(cMessage *msg)
{
}

void BVT::handleMessage(cMessage *msg)
{
//    EV << "send out from BVT " << endl;
    send(msg, "localOut");
}

void BVT::refreshDisplay() const
{
}

