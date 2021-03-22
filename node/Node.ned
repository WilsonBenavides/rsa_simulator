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
// A "Node" 
//
module Node
{
    parameters:
//        xml profile; 
        int address;
        int xpos;
        int ypos;
        
    gates:
        input portIn[];
        output portOut[];
    submodules:
        app: App {
            parameters:
                @display("p=185,43");
        }
        bvwxc: BVWXC {
            parameters:
                @display("p=88,221");
            gates:
                in[sizeof(portIn)];
                out[sizeof(portOut)];
        }
        bvt: BVT {
            parameters:
                @display("p=127,128,row");
        }
    connections:
        bvt.out --> app.in;
        bvt.in <-- app.out;
        
        bvt.localOut --> bvwxc.localIn;
        bvt.localIn <-- bvwxc.localOut;
        
        for i=0..sizeof(portIn)-1 {
			portIn[i] --> bvwxc.in[i]; 
			portOut[i] <-- bvwxc.out[i]; 
        }
        

}

