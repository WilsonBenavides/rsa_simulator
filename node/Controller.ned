//
// This program is free software: you can redistribute it and/or modify
// 

package node;

//
//
module Controller
{
    @display("p=400,40;i=block/eye_vl;bgb=144,231");

    gates:
        input in @directIn;

    submodules:
        topology: TopologyManager;
        spectrum: SpectrumManager;
    connections allowunconnected:
        in --> topology.in;
        topology.directTopoOut --> spectrum.in;
}
