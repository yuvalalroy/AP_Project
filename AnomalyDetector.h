//
// Created by yuvalalroy on 11/1/21.
//

#ifndef AP_PROJECT_ANOMALYDETECTOR_H
#define AP_PROJECT_ANOMALYDETECTOR_H
#include <string>
#include <vector>
#include <map>
#include <sstream>


class TimeSeries {
public:
    std::map<std::string, std::vector<float>> _dataMap;
    void setMapKeys(std::string keys);
    void setMapValues(std::string values);
    void chargeCSVToMap(std::istream& str);


};


class AnomalyReport {
public:
    const std::string _description;
    const long _timeStep;
    AnomalyReport(std::string description, long timeStep) : _description(description), _timeStep(timeStep){}
};

class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const TimeSeries& ts)=0;
    virtual std::vector<AnomalyReport> detect(const TimeSeries& ts)=0;
    virtual ~TimeSeriesAnomalyDetector(){}
};

#endif //AP_PROJECT_ANOMALYDETECTOR_H
