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
    types:
        channel OpticalChannel extends DelayChannel
        {
            delay = default(uniform(10us, 20us));
        }
    submodules:
        node[14]: Node {
            parameters:
                @display("i=abstract/opticalswitch;p=$xpos,$ypos;is=s");
                address = index;
        }

        controller: Controller;
    connections allowunconnected:

        node[0].portOut++ --> OpticalChannel --> node[1].portIn++; //1
        node[0].portIn++ <-- OpticalChannel <-- node[1].portOut++;

        node[0].portOut++ --> OpticalChannel --> node[2].portIn++; //2
        node[0].portIn++ <-- OpticalChannel <-- node[2].portOut++;

        node[0].portOut++ --> OpticalChannel --> node[3].portIn++; //3
        node[0].portIn++ <-- OpticalChannel <-- node[3].portOut++;

        node[1].portOut++ --> OpticalChannel --> node[6].portIn++; //4
        node[1].portIn++ <-- OpticalChannel <-- node[6].portOut++;

        node[1].portOut++ --> OpticalChannel --> node[2].portIn++; //5
        node[1].portIn++ <-- OpticalChannel <-- node[2].portOut++;

        node[2].portOut++ --> OpticalChannel --> node[7].portIn++; //6
        node[2].portIn++ <-- OpticalChannel <-- node[7].portOut++;

        node[3].portOut++ --> OpticalChannel --> node[4].portIn++; //7
        node[3].portIn++ <-- OpticalChannel <-- node[4].portOut++;

        node[3].portOut++ --> OpticalChannel --> node[8].portIn++; //8
        node[3].portIn++ <-- OpticalChannel <-- node[8].portOut++;

        node[4].portOut++ --> OpticalChannel --> node[7].portIn++; //9
        node[4].portIn++ <-- OpticalChannel <-- node[7].portOut++;

        node[4].portOut++ --> OpticalChannel --> node[5].portIn++; //10
        node[4].portIn++ <-- OpticalChannel <-- node[5].portOut++;

        node[5].portOut++ --> OpticalChannel --> node[6].portIn++; //11
        node[5].portIn++ <-- OpticalChannel <-- node[6].portOut++;

        node[6].portOut++ --> OpticalChannel --> node[9].portIn++; //12
        node[6].portIn++ <-- OpticalChannel <-- node[9].portOut++;

        node[7].portOut++ --> OpticalChannel --> node[12].portIn++; //13
        node[7].portIn++ <-- OpticalChannel <-- node[12].portOut++;

        node[7].portOut++ --> OpticalChannel --> node[11].portIn++; //14
        node[7].portIn++ <-- OpticalChannel <-- node[11].portOut++;

        node[8].portOut++ --> OpticalChannel --> node[10].portIn++; //15
        node[8].portIn++ <-- OpticalChannel <-- node[10].portOut++;

        node[8].portOut++ --> OpticalChannel --> node[13].portIn++; //16
        node[8].portIn++ <-- OpticalChannel <-- node[13].portOut++;

        node[9].portOut++ --> OpticalChannel --> node[10].portIn++; //17
        node[9].portIn++ <-- OpticalChannel <-- node[10].portOut++;

        node[9].portOut++ --> OpticalChannel --> node[13].portIn++; //18
        node[9].portIn++ <-- OpticalChannel <-- node[13].portOut++;

        node[9].portOut++ --> OpticalChannel --> node[11].portIn++; //19
        node[9].portIn++ <-- OpticalChannel <-- node[11].portOut++;

        node[12].portOut++ --> OpticalChannel --> node[13].portIn++; //20
        node[12].portIn++ <-- OpticalChannel <-- node[13].portOut++;

        node[12].portOut++ --> OpticalChannel --> node[10].portIn++; //21
        node[12].portIn++ <-- OpticalChannel <-- node[10].portOut++;
}

