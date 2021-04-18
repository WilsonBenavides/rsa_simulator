//
//

package node;

//
// Application model to generate traffic for the network.
//
simple App 
{
    parameters:
        int address;
        int slotRandomSize;
        volatile double sendIaTime @unit(s) = default(exponential(1s)); // time between generating packets
        @display("i=block/browser");
    gates:
        input in;
        output out;
}


