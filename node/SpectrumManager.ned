//
// This program is free software: you can redistribute it and/or modify
// 

package node;

//
//
simple SpectrumManager
{
    parameters:
        double slotBandwidth @unit(Hz);
        double channelBandwidth @unit(Hz);
        string assignmentAlgorithm;
    gates:
        input in @directIn;
}
