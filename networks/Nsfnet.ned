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
        channel Channel extends DelayChannel
        {
            delay = default(uniform(0.1ms, 1ms));
        }
    submodules:
        node[14]: Node {
            @display("i=abstract/opticalswitch;p=$xpos,$ypos;is=s");
        }

        controller: Controller;
    connections allowunconnected:
        

}

