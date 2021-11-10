//
// Created by amit on 04/11/2021.
//

#ifndef AP_PROJECT_SIMPLEANOMALYDETECTOR_H
#define AP_PROJECT_SIMPLEANOMALYDETECTOR_H

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
    string feature1,feature2; // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    vector<correlatedFeatures> *_matchedFeatures;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    vector<correlatedFeatures> getNormalModel();
    static Point **fromVecToPoints(vector<float> vec1, vector<float> vec2);
    void setCorrelatedFeatures(const string &f1, const vector<float> &feature1, const string &f2,
                               const vector<float> &feature2, float correlation);
};

#endif //AP_PROJECT_SIMPLEANOMALYDETECTOR_H
