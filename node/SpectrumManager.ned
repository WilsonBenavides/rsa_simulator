//
// This program is free software: you can redistribute it and/or modify
// 

package node;

//
//
simple SpectrumManager
{
    parameters:
        @display("p=39,68;i=block/spectrum_l");
        double slotBandwidth @unit(Hz);
        double channelBandwidth @unit(Hz);
        string assignmentAlgorithm;
    gates:
        input in @directIn;
}
