//
// Created by yuvalalroy on 11/1/21.
//

#ifndef AP_PROJECT_ANOMALYDETECTOR_H
#define AP_PROJECT_ANOMALYDETECTOR_H
#include "timeseries.h"
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <list>
#include <array>
#include <sstream>

using std::vector;
using std::string;

class AnomalyReport {
public:
    const string _description;
    const long _timeStep;
    AnomalyReport(string description, long timeStep) : _description(std::move(description)), _timeStep(timeStep){}
};

class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const timeseries& ts)=0;
    virtual vector<AnomalyReport> detect(const timeseries& ts)=0;
    virtual ~TimeSeriesAnomalyDetector()= default;
};

#endif //AP_PROJECT_ANOMALYDETECTOR_H
