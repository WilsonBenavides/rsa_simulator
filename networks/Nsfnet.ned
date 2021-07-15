//
// This file is part of an OMNeT++/OMNEST simulation example.
//

package networks;

import node.Node;
import node.Controller;
import ned.DelayChannel;


//
// nsfnet
//
network Nsfnet
{
    parameters:
        @display("bgb=786,562;bgi=maps/usa;bgu=km");
        double slotBandwidth @unit(Hz);
        double channelBandwidth @unit(Hz);
    types:
        channel OpticalChannel extends ned.DatarateChannel
        {
            delay = default(uniform(10us, 20us));
            datarate = default(uniform(11Gbps, 12Gbps));
        }
    submodules:
        node[14]: Node {
            parameters:
                @display("i=block/optical_s;p=$xpos,$ypos");
                address = index;
        }

        controller: Controller {
            @display("p=402.992,80.372");
            slotBandwidth = slotBandwidth;
            channelBandwidth = channelBandwidth;
        }
    connections allowunconnected:

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[0].portOut++ --> OpticalChannel --> node[1].portIn++; //1
            node[0].portIn++ <-- OpticalChannel <-- node[1].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[0].portOut++ --> OpticalChannel --> node[2].portIn++; //2
            node[0].portIn++ <-- OpticalChannel <-- node[2].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[0].portOut++ --> OpticalChannel --> node[3].portIn++; //3
            node[0].portIn++ <-- OpticalChannel <-- node[3].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[1].portOut++ --> OpticalChannel --> node[6].portIn++; //4
            node[1].portIn++ <-- OpticalChannel <-- node[6].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[1].portOut++ --> OpticalChannel --> node[2].portIn++; //5
            node[1].portIn++ <-- OpticalChannel <-- node[2].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[2].portOut++ --> OpticalChannel --> node[7].portIn++; //6
            node[2].portIn++ <-- OpticalChannel <-- node[7].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[3].portOut++ --> OpticalChannel --> node[4].portIn++; //7
            node[3].portIn++ <-- OpticalChannel <-- node[4].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[3].portOut++ --> OpticalChannel --> node[8].portIn++; //8
            node[3].portIn++ <-- OpticalChannel <-- node[8].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[4].portOut++ --> OpticalChannel --> node[7].portIn++; //9
            node[4].portIn++ <-- OpticalChannel <-- node[7].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[4].portOut++ --> OpticalChannel --> node[5].portIn++; //10
            node[4].portIn++ <-- OpticalChannel <-- node[5].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[5].portOut++ --> OpticalChannel --> node[6].portIn++; //11
            node[5].portIn++ <-- OpticalChannel <-- node[6].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[6].portOut++ --> OpticalChannel --> node[9].portIn++; //12
            node[6].portIn++ <-- OpticalChannel <-- node[9].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[7].portOut++ --> OpticalChannel --> node[12].portIn++; //13
            node[7].portIn++ <-- OpticalChannel <-- node[12].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[7].portOut++ --> OpticalChannel --> node[11].portIn++; //14
            node[7].portIn++ <-- OpticalChannel <-- node[11].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[8].portOut++ --> OpticalChannel --> node[10].portIn++; //15
            node[8].portIn++ <-- OpticalChannel <-- node[10].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[8].portOut++ --> OpticalChannel --> node[13].portIn++; //16
            node[8].portIn++ <-- OpticalChannel <-- node[13].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[9].portOut++ --> OpticalChannel --> node[10].portIn++; //17
            node[9].portIn++ <-- OpticalChannel <-- node[10].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[9].portOut++ --> OpticalChannel --> node[13].portIn++; //18
            node[9].portIn++ <-- OpticalChannel <-- node[13].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[9].portOut++ --> OpticalChannel --> node[11].portIn++; //19
            node[9].portIn++ <-- OpticalChannel <-- node[11].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[12].portOut++ --> OpticalChannel --> node[13].portIn++; //20
            node[12].portIn++ <-- OpticalChannel <-- node[13].portOut++;
        }

        for i=0..int((channelBandwidth / slotBandwidth) - 1) {
            node[12].portOut++ --> OpticalChannel --> node[10].portIn++; //21
            node[12].portIn++ <-- OpticalChannel <-- node[10].portOut++;
        }
}

