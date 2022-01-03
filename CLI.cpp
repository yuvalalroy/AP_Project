#include "CLI.h"
#include "SimpleAnomalyDetector.h"
#include "HybridAnomalyDetector.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    commands.push_back(new uploadCSVCommand(dio));
    commands.push_back(new changeThresholdCommand(dio));
    commands.push_back(new runHybridAlgoCommand(dio));
    commands.push_back(new printReportsCommand(dio));
    commands.push_back(new analyzeResultsCommand(dio));
    commands.push_back(new exitCommand(dio));
}


void CLI::start(){
    Info info;
    info.defaultThreshold = 0.9;
    info.n=0;
    string choice;
    do {
        dio->write("Welcome to the Anomaly Detection Server.\n"
                   "Please choose an option:\n"
                   "1.upload a time series csv file\n"
                   "2.algorithm settings\n"
                   "3.detect anomalies\n"
                   "4.display results\n"
                   "5.upload anomalies and analyze results\n"
                   "6.exit\n");
        choice = dio->read();
        choice.erase(std::remove(choice.begin(), choice.end(), '\n'), choice.end());
        if ((choice >= "0") && (choice <= "5")) {
            commands[stoi(choice) - 1]->execute(&info);
        }
    } while (choice != "6");

}


CLI::~CLI() {
}

