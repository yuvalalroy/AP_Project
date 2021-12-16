#include "CLI.h"
#include "SimpleAnomalyDetector.h"
#include "HybridAnomalyDetector.h"

CLI::CLI(DefaultIO* dio) {
    *dio = *dio;
}


void changeThreshold(DefaultIO* dio) {
    float *newThreshold;
    while (true) {
        dio->write("The current correlation threshold is 0.9\n");
        dio->read(newThreshold);
        if ((*newThreshold > 0) && (*newThreshold < 1)) {
            SimpleAnomalyDetector::setThreshold(*newThreshold);
            break;
        }
        dio->write("please choose a value between 0 and 1.\n");
    }
}

void uploadCSV(DefaultIO* dio, const string& name, const string& request) {
    dio->write(request);
    vector<string> text;
    string line;
    do {
        line = dio->read();
        text.push_back(line);
    } while (line != "done");

    dio->uploadFiles(name, text);
    dio->write("Upload complete.");
}

void runHybridAlgo(DefaultIO* dio){
    HybridAnomalyDetector ad;

    ad.learnNormal()
}

void CLI::start(){
    dio->write("Welcome to the Anomaly Detection Server. \n"
               "Please choose an option: \n"
               "1. upload a time series csv file \n"
               "2. algorithm settings \n"
               "3. detect anomalies \n"
               "4. display results \n"
               "5. upload anomalies and analyze results \n"
               "6. exit");
    string choice = dio->read();

    switch (std::stoi(choice)) {
        case 1:
            uploadCSV(dio, "anomalyTrain.csv", "Please upload your local train CSV file.\n");
            uploadCSV(dio, "anomalyTest.csv", "Please upload your local test CSV file.\n");
        case 2:
            changeThreshold(dio);
        case 3:
            runHybridAlgo(dio);
        case 4:
        case 5:
        case 6:

    }
}


CLI::~CLI() {
}

