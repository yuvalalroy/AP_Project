

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

struct Info {
    float defaultThreshold;
    vector<AnomalyReport> to_report;
    vector<pair<pair<long, long>, pair<string, bool>>> bindReports;
    long n;
};

// you may edit this class
class Command {
protected:
    DefaultIO *dio;


public:
    Command(DefaultIO *dio) : dio(dio) {}
    virtual void execute(Info* info) = 0;
    virtual ~Command() {}

    void uploadFilesToServer(const string &fileName, const vector<string> &text) {
        std::ofstream file;
        file.open(fileName);
        for (const string &line: text) {
            file << line + "\n";
        }
        file.close();
    }

    void uploadFile(DefaultIO* dio, const string& name, const string& request, Info* info) {
        dio->write(request);
        vector<string> text;
        string line = dio->read();
        while (line != "done\n") {
            text.push_back(line);
            line = dio->read();
            if (name == "anomalyTest.csv")
                info->n = info->n+1;
        }
        info->n = info->n-1;
        uploadFilesToServer(name, text);
        dio->write("Upload complete.\n");
    }

};

class uploadCSVCommand:virtual public Command{
public:
    explicit uploadCSVCommand(DefaultIO* dio):Command(dio){}

    void execute(Info* info) override{
        uploadFile(dio, "anomalyTrain.csv", "Please upload your local train CSV file.\n", info);
        uploadFile(dio, "anomalyTest.csv", "Please upload your local test CSV file.\n", info);
    }

    virtual ~uploadCSVCommand(){}
};

class changeThresholdCommand:virtual public Command{
public:
    explicit changeThresholdCommand(DefaultIO* dio):Command(dio){}

    void execute(Info* info) override{
        float newThreshold;
        dio->write("The current correlation threshold is ");
        dio->write(info->defaultThreshold);
        dio->write("\n");
        dio->write("Type a new threshold\n");
        while (true) {
            dio->read(&newThreshold);
            if ((newThreshold >= 0) && (newThreshold <= 1)) {
                info->defaultThreshold = newThreshold;
                break;
            }
            dio->write("please choose a value between 0 and 1.\n");
        }
    }

    virtual ~changeThresholdCommand(){}
};

class runHybridAlgoCommand:virtual public Command{
public:
    explicit runHybridAlgoCommand(DefaultIO* dio):Command(dio){}

    void execute(Info* info) override {
        HybridAnomalyDetector ad;
        TimeSeries trainCSV("anomalyTrain.csv");
        TimeSeries testCSV("anomalyTest.csv");
        HybridAnomalyDetector::setThreshold(info->defaultThreshold);
        ad.learnNormal(trainCSV);
        info->to_report = ad.detect(testCSV);

        dio->write("anomaly detection complete.\n");
        //info->n = testCSV.getN();
    }
};

class printReportsCommand:virtual public Command {
public:
    explicit printReportsCommand(DefaultIO* dio):Command(dio){}

    void execute(Info* info) override {
        for (const AnomalyReport& report: info->to_report){
            dio->write((float)report.timeStep);
            dio->write("\t" + report.description + "\n");
        }
        dio->write("Done.\n");
    }
};

class analyzeResultsCommand:virtual public Command {
public:
    explicit analyzeResultsCommand(DefaultIO *dio) : Command(dio) {}

    void bind(Info* info){
        pair<pair<long, long>, pair<string,bool>> tmp;

        for_each(info->to_report.begin(), info->to_report.end(), [&tmp, info](AnomalyReport& report){
            if((report.timeStep == tmp.first.second + 1) && (report.description == tmp.second.first))
                tmp.first.second ++;
            else{
                info->bindReports.push_back(tmp);
                tmp.first.first= report.timeStep;
                tmp.first.second = tmp.first.first;
                tmp.second.first = report.description;
            }
        });
        info->bindReports.push_back(tmp);
        info->bindReports.erase(info->bindReports.begin());
    }

    bool truePositive(pair<long, long> p, Info* info) {
        for(pair<pair<long,long>, pair<string, bool>> &pair : info->bindReports) {
            if ((p.second >= pair.first.first) && (p.first <= pair.first.second)) {
                pair.second.second = true;
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

    void execute(Info* info) override {
        bind(info);
        for(pair<pair<long,long>, pair<string, bool>> &pair : info->bindReports){
            pair.second.second = false;
        }
        dio->write("Please upload your local anomalies file.\n");
        vector<pair<long, long>> data;
        stringstream TPstream, FPstream;
        string line;
        line = dio->read();
        while (line != "done\n") {
            data.push_back(split(line));
            line = dio->read();
        }
        dio->write("Upload complete.\n");
        float TP=0, FP = 0;
        size_t P = data.size();

        long sum = 0;
        for (pair<long, long> pair: data) {
            if(truePositive(pair, info))
                TP++;
            sum += pair.second - pair.first + 1;
        }
        for (pair<pair<long,long>, pair<string, bool>> &pair : info->bindReports) {
            if (!pair.second.second)
                FP++;
        }

        long N = info->n - sum;
//        float truePosRate = ((int)(1000.0*TP/P))/1000.0f;
//        float falseAlarmRate = ((int)(1000.0*FP/N))/1000.0f;

        float truePosRate = (float) floor(TP * 1000 / P) / 1000;
        float falseAlarmRate = (float) floor(FP * 1000 / N) / 1000;
        TPstream << truePosRate;
        FPstream << falseAlarmRate;

        dio->write("True Positive Rate: ");
        dio->write(TPstream.str());
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(FPstream.str());
        dio->write("\n");

        info->bindReports.clear();
    }
};

class exitCommand:virtual public Command {
public:
    explicit exitCommand(DefaultIO* dio):Command(dio){}
    void execute(Info* info) override {}
};


#endif /* COMMANDS_H_ */
