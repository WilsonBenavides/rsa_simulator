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
    //		**.node[*].address = 0;		
    types:
        channel Channel extends DelayChannel
        {
            delay = default(uniform(0.1ms, 1ms));
        }
    submodules:
        node[14]: Node {
            address = index;
        }

        controller: Controller;
        seattle: Node {
            @display("p=76,51");
        }
        palo_alto: Node {
            address = 1;
            @display("p=21,192");
        }
        san_diego: Node {
            address = 2;
            @display("p=104,325");
        }
        champaign: Node {
            address = 3;
            @display("p=443,266");
        }
        salt_lake: Node {
            address = 4;
            @display("p=187,245");
        }
        houston: Node {
            address = 5;
            @display("p=419,408");
        }
        ann_arbor: Node {
            address = 6;
            @display("p=523,183");
        }
        boulder: Node {
            address = 7;
            @display("p=283,214");
        }
        lincoln: Node {
            address = 8;
            @display("p=357,252");
        }
        atlanta: Node {
            address = 9;
            @display("p=568,356");
        }
        college_pk: Node {
            address = 10;
            @display("p=658,266");
        }
        pittsburg: Node {
            address = 11;
            @display("p=562,260");
        }
        princeton: Node {
            address = 12;
            @display("p=658,214");
        }
        ithaca: Node {
            address = 13;
            @display("p=595,183");
        }
    connections allowunconnected:
        node[0].portOut++ --> Channel --> palo_alto.portIn++;
        node[0].portIn++ <-- Channel <-- palo_alto.portOut++;

        node[0].portOut++ --> Channel --> san_diego.portIn++;
        node[0].portIn++ <-- Channel <-- san_diego.portOut++;

        node[0].portOut++ --> Channel --> champaign.portIn++;
        node[0].portIn++ <-- Channel <-- champaign.portOut++;

        palo_alto.portOut++ --> Channel --> salt_lake.portIn++;
        palo_alto.portIn++ <-- Channel <-- salt_lake.portOut++;

        palo_alto.portOut++ --> Channel --> san_diego.portIn++;
        palo_alto.portIn++ <-- Channel <-- san_diego.portOut++;

        san_diego.portOut++ --> Channel --> houston.portIn++;
        san_diego.portIn++ <-- Channel <-- houston.portOut++;

        salt_lake.portOut++ --> Channel --> ann_arbor.portIn++;
        salt_lake.portIn++ <-- Channel <-- ann_arbor.portOut++;

        salt_lake.portOut++ --> Channel --> boulder.portIn++;
        salt_lake.portIn++ <-- Channel <-- boulder.portOut++;

        boulder.portOut++ --> Channel --> houston.portIn++;
        boulder.portIn++ <-- Channel <-- houston.portOut++;

        boulder.portOut++ --> Channel --> lincoln.portIn++;
        boulder.portIn++ <-- Channel <-- lincoln.portOut++;

        houston.portOut++ --> Channel --> atlanta.portIn++;
        houston.portIn++ <-- Channel <-- atlanta.portOut++;

        houston.portOut++ --> Channel --> college_pk.portIn++;
        houston.portIn++ <-- Channel <-- college_pk.portOut++;

        lincoln.portOut++ --> Channel --> champaign.portIn++;
        lincoln.portIn++ <-- Channel <-- champaign.portOut++;

        champaign.portOut++ --> Channel --> pittsburg.portIn++;
        champaign.portIn++ <-- Channel <-- pittsburg.portOut++;

        pittsburg.portOut++ --> Channel --> atlanta.portIn++;
        pittsburg.portIn++ <-- Channel <-- atlanta.portOut++;

        pittsburg.portOut++ --> Channel --> princeton.portIn++;
        pittsburg.portIn++ <-- Channel <-- princeton.portOut++;

        pittsburg.portOut++ --> Channel --> ithaca.portIn++;
        pittsburg.portIn++ <-- Channel <-- ithaca.portOut++;

        ann_arbor.portOut++ --> Channel --> ithaca.portIn++;
        ann_arbor.portIn++ <-- Channel <-- ithaca.portOut++;

        ann_arbor.portOut++ --> Channel --> princeton.portIn++;
        ann_arbor.portIn++ <-- Channel <-- princeton.portOut++;

        ithaca.portOut++ --> Channel --> college_pk.portIn++;
        ithaca.portIn++ <-- Channel <-- college_pk.portOut++;

        princeton.portOut++ --> Channel --> college_pk.portIn++;
        princeton.portIn++ <-- Channel <-- college_pk.portOut++;
}

