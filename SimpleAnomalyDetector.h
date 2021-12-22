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

struct correlatedFeatures {
    string feature1,feature2; // names of the correlated features
    Line lin_reg;
    Point center;
    float corrlation;
    float threshold;
    float radius;
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
    vector<correlatedFeatures> *_matchedFeatures;
    static float _threshold;
public:

    // constructor
    SimpleAnomalyDetector();

    // destructor
    virtual ~SimpleAnomalyDetector();

    // learn the normal model by the time-series map as an input
    virtual void learnNormal(const TimeSeries& ts);

    // detect the correlations and the _reports
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    // getter for the normal model
    vector<correlatedFeatures> getNormalModel();

    // get a points array from two vectors
    static Point **fromVecToPoints(vector<float> vec1, vector<float> vec2);

    virtual void setCorrelatedFeatures(const string &f1, const vector<float> &feature1, const string &f2,
                               const vector<float> &feature2, float correlation);

    void setMatchedFeatures(const vector<correlatedFeatures>& matched);

    static void setThreshold(const float &threshold);
};

#endif //AP_PROJECT_SIMPLEANOMALYDETECTOR_H
