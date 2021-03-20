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
        routing: TopologyManager;
        spectrum: SpectrumManager;
        display: DisplayManager;
        rsa: DynamicRSA;
    connections allowunconnected:
	in --> rsa.in;
}
