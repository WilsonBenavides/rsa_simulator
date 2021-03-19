//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

package node;

//
// Represents the network interface for a point-to-point connection.
//
simple BVT
{
    parameters:
        @display("i=block/queue;q=queue");
    gates:
        input in;
        output out;
        inout line;
}


