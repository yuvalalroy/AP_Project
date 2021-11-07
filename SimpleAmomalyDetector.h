//
// Created by amit on 04/11/2021.
//

#ifndef AP_PROJECT_SIMPLEAMOMALYDETECTOR_H
#define AP_PROJECT_SIMPLEAMOMALYDETECTOR_H

#include "anomaly_detection_util.h"

struct correlatedFeatures{
    string feature1,feature2; // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    vector<correlatedFeatures> matchedFeatures;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    vector<correlatedFeatures> getNormalModel();
    void setCorrelatedFeatures(const vector<float> &feature1, const vector<float> &feature2, float corrlation);

    static Point **fromVecToPoints(vector<float> vec1, vector<float> vec2);
};

#endif //AP_PROJECT_SIMPLEAMOMALYDETECTOR_H
