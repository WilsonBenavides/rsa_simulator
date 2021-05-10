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
        volatile int  packetLength @unit(byte) = default(100B); //lenght of one packet
        @display("i=block/browser");
    gates:
        input in;
        output out;
}


