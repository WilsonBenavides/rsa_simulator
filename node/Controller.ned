//
// This program is free software: you can redistribute it and/or modify
// 

package node;

//
//
module Controller
{
    @display("p=400,40;i=block/star");

    gates:
        input in @directIn;

    submodules:
        display: DisplayManager;
        topology: TopologyManager;
        spectrum: SpectrumManager {

            @display("p=379,83");
        }
    connections allowunconnected:
        in --> topology.in;
        topology.directTopoOut --> spectrum.in;
}
