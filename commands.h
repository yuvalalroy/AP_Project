

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
};

//class standardIO:public DefaultIO {
//    string read() override {
//        string input;
//        cin >> input;
//        return input;
//    }
//
//    void write(string text) override {
//        cout << text;
//    }
//
//    void write(float f) override {
//        cout << f;
//    }
//
//    void read(float* f) override {
//        cin >> *f;
//    }
//};
//
//class socketIO:public DefaultIO {
//    virtual string read();
//    virtual void write(string text);
//    virtual void write(float f);
//    virtual void read(float* f);
//};
//

// you may edit this class
class Command {
protected:
    DefaultIO *dio;
    float _threshold;
    vector<AnomalyReport> _reports;
    vector<pair<pair<long, long>, string>> _bindReports;

public:
    Command(DefaultIO *dio) : dio(dio) {
        _threshold = 0.9;
    }
    virtual void execute() = 0;
    virtual ~Command() {}

    void uploadFilesToServer(const string &fileName, const vector<string> &text) {
        std::ofstream file;
        file.open(fileName);
        for (const string &line: text) {
            file << line + "\n";
        }
        file.close();
    }

    void uploadFile(DefaultIO* dio, const string& name, const string& request) {
        dio->write(request);
        vector<string> text;
        string line;
        do {
            line = dio->read();
            text.push_back(line);
        } while (line != "done");

        uploadFilesToServer(name, text);
        dio->write("Upload complete.");
    }

    void setThreshold(const float& threshold){
        _threshold = threshold;
    }
};

class uploadCSVCommand:public Command{
public:
    explicit uploadCSVCommand(DefaultIO* dio):Command(dio){}

    void execute() override{
        uploadFile(dio, "anomalyTrain.csv", "Please upload your local train CSV file.\n");
        uploadFile(dio, "anomalyTest.csv", "Please upload your local test CSV file.\n");
    }

    virtual ~uploadCSVCommand(){}
};

class changeThresholdCommand:public Command{
public:
    explicit changeThresholdCommand(DefaultIO* dio):Command(dio){}

    void execute() override{
        float newThreshold;
        while (true) {
            dio->write("The current correlation threshold is 0.9\n");
            dio->read(&newThreshold);
            if ((newThreshold > 0) && (newThreshold < 1)) {
                setThreshold(newThreshold);
                break;
            }
            dio->write("please choose a value between 0 and 1.\n");
        }
    }

    virtual ~changeThresholdCommand(){}
};

class runHybridAlgoCommand:public Command{
public:
    explicit runHybridAlgoCommand(DefaultIO* dio):Command(dio){}

    void bindReports(){
        pair<pair<long, long>, string> tmp;
        for_each(_reports.begin(),_reports.end(),[&tmp, this](AnomalyReport& report){
            if((report.timeStep == tmp.first.second + 1) && (report.description == tmp.second))
                tmp.first.second ++;
            else{
                _bindReports.push_back(tmp);
                tmp.first.first= report.timeStep;
                tmp.first.second = tmp.first.first;
                tmp.second = report.description;

            }
        });
        _bindReports.push_back(tmp);
        //_bindReports.erase(_bindReports.begin());
    }

    void execute() override {
        HybridAnomalyDetector ad;
        TimeSeries trainCSV("/home/amit/CLionProjects/AP_Project/anomalyTrain.csv");
        TimeSeries testCSV("/home/amit/CLionProjects/AP_Project/anomalyTest.csv");
        HybridAnomalyDetector::setThreshold(_threshold);
        ad.learnNormal(trainCSV);
        _reports = ad.detect(testCSV);
        bindReports();
        dio->write("anomaly detection complete.\n");
    }
};

class printReportsCommand:public Command {
public:
    explicit printReportsCommand(DefaultIO* dio):Command(dio){}

    void execute() override {
        for (const AnomalyReport& report: _reports){
            dio->write((float)report.timeStep);
            dio->write("\t");
            dio->write(report.description + "\n");
        }
        dio->write("Done.\n");
    }
};

class analyzeResultsCommand:public Command {
public:
    explicit analyzeResultsCommand(DefaultIO *dio) : Command(dio) {}

    bool truePositive(pair<long, long> p) {
        for(const pair<pair<long,long>, string>& pair : _bindReports) {
           if ((p.first >= pair.first.first) && (p.second >= pair.first.second)){
               return true;
           }
        }
        return false;
    }

    pair<long, long> split(const string &str) {
        pair<long, long> pair;
        string substr;
        stringstream s(str);
        getline(s, substr, ',');
        pair.first = (long) stoi(substr);
        getline(s, substr, ',');
        pair.second = (long) stoi(substr);

        return pair;
    }

    void execute() override {
        dio->write("Please upload your local anomalies file.\n");
        vector<pair<long, long>> data;
        string line;
        do {
            line = dio->read();
            data.push_back(split(line));
        } while (line != "done");
        dio->write("Upload complete.");
        float TP, FP = 0;
        size_t P = data.size();
        long n = data.end()->second - data.begin()->first + 1;
        long sum = 0;
        for (pair<long, long> pair: data) {
            if(truePositive(pair))
                TP++;
            else
                FP++;
            sum += pair.second - pair.first + 1;
        }

        long N = n - sum;
        float truePosRate =  floorf((TP / (float)P) * 1000) / 1000 ;
        float falseAlarmRate =  floorf((FP / (float)N) * 1000) / 1000 ;
        dio->write("True Positive Rate: ");
        dio->write(truePosRate);
        dio->write("False Positive Rate: ");
        dio->write(falseAlarmRate);
    }
};

class exitCommand:public Command {
public:
    explicit exitCommand(DefaultIO* dio):Command(dio){}
    void execute() override {}
};


#endif /* COMMANDS_H_ */
