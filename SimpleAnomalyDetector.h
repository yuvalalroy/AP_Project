// Amit Paz ID:319003455
// Yuval Alroy ID:315789461

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

    // constructor
    SimpleAnomalyDetector();

    // destructor
    virtual ~SimpleAnomalyDetector();

    // learn the normal model by the time-series map as an input
    virtual void learnNormal(const TimeSeries& ts);

    // detect the correlations and the reports
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    // getter for the normal model
    vector<correlatedFeatures> getNormalModel();

    // get a points array from two vectors
    static Point **fromVecToPoints(vector<float> vec1, vector<float> vec2);

    // set the _matchedFeatures field that will contain the correlated features
    void setCorrelatedFeatures(const string &f1, const vector<float> &feature1, const string &f2,
                               const vector<float> &feature2, float correlation);
};

#endif //AP_PROJECT_SIMPLEANOMALYDETECTOR_H
