//
// Created by yuvalalroy on 11/1/21.
//

#ifndef AP_PROJECT_ANOMALYDETECTOR_H
#define AP_PROJECT_ANOMALYDETECTOR_H
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <list>
#include <array>
#include <sstream>

using std::vector;
using std::string;

class TimeSeries {
    std::map<string, vector<float>> _dataMap;
public:
    void setMapKeys(const string& keys);
    void setMapValues(const string& values);
    void chargeCSVToMap(const char* CSVFile);
    vector<string> getFeatures() const;
    vector<float> getVecByFeature(const string &feature) const;
};


class AnomalyReport {
public:
    const string _description;
    const long _timeStep;
    AnomalyReport(string description, long timeStep) : _description(std::move(description)), _timeStep(timeStep){}
};

class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const TimeSeries& ts)=0;
    virtual vector<AnomalyReport> detect(const TimeSeries& ts)=0;
    virtual ~TimeSeriesAnomalyDetector(){}
};

#endif //AP_PROJECT_ANOMALYDETECTOR_H
