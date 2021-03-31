//
// This file is part of an OMNeT++/OMNEST simulation example.
//
#include <sstream>
#include <iostream>
#include <fstream>
#include <omnetpp.h>
#include "OpticalMsg_m.h"

using namespace omnetpp;

/**
 * Demonstrates static routing, utilizing the cTopology class.
 */
class BVWXC : public cSimpleModule
{
private:
    typedef std::map<int, int> RoutingTable;  // destaddr -> gateindex
    std::map<int, int> rtable;

protected:
    virtual void forwardMessage(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    std::vector<std::pair<std::string, std::vector<int>>> read_csv(std::string filename);
    void write_csv(std::string filename, std::vector<std::pair<std::string, std::vector<int>>> dataset);
};

Define_Module(BVWXC);

void BVWXC::initialize()
{
}

void BVWXC::handleMessage(cMessage *msg)
{
    int index = getParentModule()->par("address");
    OpticalMsg *opmsg = check_and_cast<OpticalMsg*>(msg);
    int ss = getParentModule()->par("address");
    EV << "address : " << ss << endl;
    if (opmsg->getDestAddr() == ss) {
        //Message arrived
        bubble("message arrived!");
        EV << "Message : " << msg << "  arrived!\n";
        delete msg;
    }
    else {
        forwardMessage(msg);
    }
}

void BVWXC::forwardMessage(cMessage *msg)
{
    EV << "msg send to controller : " << getParentModule()->getParentModule()->getSubmodule("controller")->getName() << endl;
    cModule *control = getParentModule()->getParentModule()->getSubmodule("controller");
    sendDirect(msg, control, "in");

//    OpticalMsg *opmsg = check_and_cast<OpticalMsg*>(msg);
//    int srcAddress = opmsg->getSrcAddr();
//    int dstAddress = opmsg->getDestAddr();
//    EV << "source address " << srcAddress << endl;
//    int num = getParentModule()->par("address");
//    std::string csvName = "./node/LocalRouting" + std::to_string(num) + ".csv";
//    std::vector<std::pair<std::string, std::vector<int>>> two_cols = read_csv(csvName);
//
//    std::vector<int> gates, dstAddr;
//
//    int stop = 13;
//    for (int j = 0; j < two_cols.size(); j++) {
//        for (int i = 0; i < stop; i++) {
//
//            if (j == 1) {
////                EV << "Second : " << two_cols.at(j).second.at(i) << endl;
//                gates.push_back(two_cols.at(j).second.at(i));
//            }
//
//            if (j == 0) {
////                EV << "Second : " << two_cols.at(j).second.at(i) << endl;
//                dstAddr.push_back(two_cols.at(j).second.at(i));
//            }
//
//        }
//    }
//    for (int i = 0; i < gates.size(); ++i)
//        rtable[dstAddr[i]] = gates[i];
//
//    RoutingTable::iterator it = rtable.find(dstAddress);
//    int outGateIndex = (*it).second;
//
//    EV << "forwarding packet " << opmsg->getName() << " on gate index " << outGateIndex << endl;
//    send(opmsg, "out", outGateIndex);

}

std::vector<std::pair<std::string, std::vector<int>>> BVWXC::read_csv(std::string filename)
{
    // Reads a CSV file into a vector of <string, vector<int>> pairs where
    // each pair represents <column name, column values>

    // Create a vector of <string, int vector> pairs to store the result
    std::vector<std::pair<std::string, std::vector<int>>> result;

    // Create an input filestream
    std::ifstream myFile(filename);

    // Make sure the file is open
    if (!myFile.is_open())
        throw std::runtime_error("Could not open file");

    // Helper vars
    std::string line, colname;
    int val;

    // Read the column names
    if (myFile.good()) {
        // Extract the first line in the file
        std::getline(myFile, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each column name
        while (std::getline(ss, colname, ',')) {

            // Initialize and add <colname, int vector> pairs to result
            result.push_back( { colname, std::vector<int> { } });
        }
    }

    // Read data, line by line
    while (std::getline(myFile, line)) {
        // Create a stringstream of the current line
        std::stringstream ss(line);

        // Keep track of the current column index
        int colIdx = 0;

        // Extract each integer
        while (ss >> val) {

            // Add the current integer to the 'colIdx' column's values vector
            result.at(colIdx).second.push_back(val);

            // If the next token is a comma, ignore it and move on
            if (ss.peek() == ',')
                ss.ignore();

            // Increment the column index
            colIdx++;
        }
    }

    // Close file
    myFile.close();

    return result;
}

void BVWXC::write_csv(std::string filename, std::vector<std::pair<std::string, std::vector<int>>> dataset)
{
    // Make a CSV file with one or more columns of integer values
    // Each column of data is represented by the pair <column name, column data>
    //   as std::pair<std::string, std::vector<int>>
    // The dataset is represented as a vector of these columns
    // Note that all columns should be the same size

    // Create an output filestream object
    std::ofstream myFile(filename);

    // Send column names to the stream
    for (int j = 0; j < dataset.size(); ++j) {
        myFile << dataset.at(j).first;
        if (j != dataset.size() - 1)
            myFile << ","; // No comma at end of line
    }
    myFile << "\n";

    // Send data to the stream
    for (int i = 0; i < dataset.at(0).second.size(); ++i) {
        for (int j = 0; j < dataset.size(); ++j) {
            myFile << dataset.at(j).second.at(i);
            if (j != dataset.size() - 1)
                myFile << ","; // No comma at end of line
        }
        myFile << "\n";
    }

    // Close the file
    myFile.close();
}
