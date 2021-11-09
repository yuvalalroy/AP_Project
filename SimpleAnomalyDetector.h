//
// Created by amit on 04/11/2021.
//

#ifndef AP_PROJECT_SIMPLEANOMALYDETECTOR_H
#define AP_PROJECT_SIMPLEANOMALYDETECTOR_H

#include "anomaly_detection_util.h"

struct correlatedFeatures{
    string feature1,feature2; // names of the correlated features
    float correlation;
    Line lin_reg;
    float threshold;
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    vector<correlatedFeatures> *_matchedFeatures;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    void learnNormal(const timeseries& ts) override;
    virtual vector<AnomalyReport> detect(const timeseries& ts);
    vector<correlatedFeatures> getNormalModel();
    static Point **fromVecToPoints(vector<float> vec1, vector<float> vec2);
    void setCorrelatedFeatures(const string &f1, const vector<float> &feature1, const string &f2,
                               const vector<float> &feature2, float correlation);
};

#endif //AP_PROJECT_SIMPLEANOMALYDETECTOR_H
