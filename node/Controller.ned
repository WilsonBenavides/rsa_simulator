//
// This program is free software: you can redistribute it and/or modify
// 

package node;

//
//
module Controller
{
    parameters:
        @display("p=400,40;i=block/eye_vl;bgb=144,231");
        double slotBandwidth @unit(Hz);
        double channelBandwidth @unit(Hz);

    gates:
        input in @directIn;

    submodules:
        topology: TopologyManager;
        spectrum: SpectrumManager {
        	slotBandwidth = slotBandwidth;
        	channelBandwidth = channelBandwidth;
        }
    connections allowunconnected:
        in --> topology.in;
        topology.directTopoOut --> spectrum.in;
}
