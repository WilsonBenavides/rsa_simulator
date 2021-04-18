//
// This program is free software: you can redistribute it and/or modify
// 

package node;

//
//
simple SpectrumManager
{
    parameters:
        double slotBandwidth;
        double channelBandwidth;
        string assignmentAlgorithm;
    gates:
        input in @directIn;
}
