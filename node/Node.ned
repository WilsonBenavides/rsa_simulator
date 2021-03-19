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
// A "Node" consists of a Routing plus an App module which represents the
// aggregate traffic of the subnets connected to that router.
//
module Node
{
    parameters:
        @display("i=abstract/opticalswitch;is=s");
    gates:
        inout port[];
    submodules:
        app: App {
            parameters:
                @display("p=185,43");
        }
        bvwxc: BVWXC {
            parameters:
                @display("p=88,221");
            gates:
                line[sizeof(port)];
        }
        bvt: BVT {
            parameters:
                @display("p=127,128,row");
        }
    connections:
        bvt.out --> app.in;
        bvt.in <-- app.out;
        bvt.line <--> bvwxc.localLine;
        
        for i=0..sizeof(port)-1 {
            bvwxc.line[i] <--> port[i];
        }
}


