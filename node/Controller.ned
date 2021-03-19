//
// This program is free software: you can redistribute it and/or modify
// 

package node;

//
//
module Controller
{
    @display("p=400,40;i=block/star");
    
    submodules:
        topology : TopologyManager;
        spectrum : SpectrumManager;
        display :  DisplayManager;
}
