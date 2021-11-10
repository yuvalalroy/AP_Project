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
#include "math.h"

using std::vector;
using std::string;

class AnomalyReport {
public:
    const string description;
    const long timeStep;
    AnomalyReport(string description, long timeStep) : description(std::move(description)), timeStep(timeStep){}
};

class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const TimeSeries& ts)=0;
    virtual vector<AnomalyReport> detect(const TimeSeries& ts)=0;
    virtual ~TimeSeriesAnomalyDetector()= default;
};

#endif //AP_PROJECT_ANOMALYDETECTOR_H
