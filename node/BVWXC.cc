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
    std::pair<int, int> rtable;

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
    if (index == 10) {
        //Message arrived
        EV << "Message : " << msg << "  arrived!\n";
        delete msg;
    }
    else {
        forwardMessage(msg);
    }
}

void BVWXC::forwardMessage(cMessage *msg)
{
//    int n = gateSize("line$o");
//    int k = intuniform(0, n - 1);

//    EV << "forwarding message " << msg << " on port out[" << k << " ]\n";
//    EV << "getId " << getId() << endl;
//    EV << "getfullpath " << getFullPath() << endl;
//    send(msg, "line$o", k);

//    EV << "msg send to controller : " << getParentModule()->getParentModule()->getSubmodule("controller")->getName() << endl;
//    cModule *control = getParentModule()->getParentModule()->getSubmodule("controller");
//    sendDirect(msg, control, "in");

    OpticalMsg *opmsg = check_and_cast<OpticalMsg*>(msg);
    int srcAddress = opmsg->getSrcAddr();
    int dstAddress = opmsg->getDestAddr();
    std::vector<std::string> dstGate;
    EV << "source address " << srcAddress << endl;

    std::string myText;
    std::ifstream myReadFile("./node/LocalRouting0.txt");

    while (std::getline(myReadFile, myText)) {
        EV << myText << endl;
        std::stringstream ss(myText);
        std::string tmp;
        int dstaddr = 0;
        int gate = 0;
        while (std::getline(ss, tmp, ',')) {
            dstGate.push_back(tmp);
        }
    }

    std::vector<std::pair<std::string, std::vector<int>>> two_cols = read_csv("./node/LocalRouting0.csv");

    EV << "col 1 : " << two_cols.at(0).first << "  col 2 : " << two_cols.at(1).first << endl;
    EV << two_cols.at(0).second.at(0) << " :   ,   : " << two_cols.at(1).second.at(0) << endl;
    EV << two_cols.at(0).second.at(1) << " :   ,   : " << two_cols.at(1).second.at(1) << endl;
    EV << two_cols.at(0).second.at(2) << " :   ,   : " << two_cols.at(1).second.at(2) << endl;
    EV << two_cols.at(0).second.at(3) << " :   ,   : " << two_cols.at(1).second.at(3) << endl;

    int stop = 13;
    for (int j = 0; j < two_cols.size(); j++) {
        for (int i = 0; i < stop; i++) {

            EV << "Second : " << two_cols.at(j).second.at(i) << endl;
        }
    }



    myReadFile.close();

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
