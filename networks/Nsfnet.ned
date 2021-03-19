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
    @display("bgb=786,562;bgi=maps/usa;bgu=km");
    types:
        channel Channel extends DelayChannel
        {
            delay = default(uniform(0.1ms, 1ms));
        }
    submodules:
        controller: Controller {
            
        }
        
        seattle: Node {
            @display("p=76,51");
        }
        palo_alto: Node {
            @display("p=21,192");
        }
        san_diego: Node {
            @display("p=104,325");
        }
        champaign: Node {
            @display("p=443,266");
        }
        salt_lake: Node {
            @display("p=187,245");
        }
        houston: Node {
            @display("p=419,408");
        }
        ann_arbor: Node {
            @display("p=523,183");
        }
        boulder: Node {
            @display("p=283,214");
        }
        lincoln: Node {
            @display("p=357,252");
        }
        atlanta: Node {
            @display("p=568,356");
        }
        college_pk: Node {
            @display("p=658,266");
        }
        pittsburg: Node {
            @display("p=562,260");
        }
        princeton: Node {
            @display("p=658,214");
        }
        ithaca: Node {
            @display("p=595,183");
        }
    connections:
        seattle.port++ <--> Channel <--> palo_alto.port++;
        seattle.port++ <--> Channel <--> san_diego.port++;
        seattle.port++ <--> Channel <--> champaign.port++;

        palo_alto.port++ <--> Channel <--> salt_lake.port++;
        palo_alto.port++ <--> Channel <--> san_diego.port++;

        san_diego.port++ <--> Channel <--> houston.port++;

        salt_lake.port++ <--> Channel <--> ann_arbor.port++;
        salt_lake.port++ <--> Channel <--> boulder.port++;

        boulder.port++ <--> Channel <--> houston.port++;
        boulder.port++ <--> Channel <--> lincoln.port++;

        houston.port++ <--> Channel <--> atlanta.port++;
        houston.port++ <--> Channel <--> college_pk.port++;

        lincoln.port++ <--> Channel <--> champaign.port++;

        champaign.port++ <--> Channel <--> pittsburg.port++;

        pittsburg.port++ <--> Channel <--> atlanta.port++;
        pittsburg.port++ <--> Channel <--> princeton.port++;
        pittsburg.port++ <--> Channel <--> ithaca.port++;

        ann_arbor.port++ <--> Channel <--> ithaca.port++;
        ann_arbor.port++ <--> Channel <--> princeton.port++;

        ithaca.port++ <--> Channel <--> college_pk.port++;

        princeton.port++ <--> Channel <--> college_pk.port++;
}

