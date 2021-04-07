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
        spectrum: SpectrumManager;
        display: DisplayManager;
        rsa_control: DynamicRSA;
        routing: TopologyManager {

            @display("p=379,83");
        }
    connections allowunconnected:
        in --> rsa_control.in;
        rsa_control.directTopoOut --> routing.in;
}
