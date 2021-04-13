//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


//
// Represents a packet in the network.
//

packet OpticalMsg
{
    int srcAddr;
    int destAddr;
    int slotReq;
    int msgState;
    int red;
    int green;
    int blue;
}

packet OpticalMsgPath
{
    int srcAddr;
    int destAddr;
    int slotReq;
    int msgState;
    int opticalPath[];
    int red;
    int green;
    int blue;
}

