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
// Models a router.
//
simple BVWXC
{
    parameters:
        @display("i=block/switch");
    gates:
        input directIn @directIn;
        input in[];
        output out[];
        
        input localIn;
        output localOut;
}


