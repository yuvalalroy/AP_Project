// Amit Paz ID:319003455
// Yuval Alroy ID:315789461
#include "timeseries.h"
#include "minCircle.h"
#include "SimpleAnomalyDetector.h"
#include "HybridAnomalyDetector.h"

float SimpleAnomalyDetector::_threshold;

// learn the normal model by the time-series map as an input
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    vector<string> mapFeatures = ts.getFeatures();
    unsigned int sizeOfFeatures = mapFeatures.size();
    for (int i = 0; i < sizeOfFeatures; i++) {
        float maxPearson = 0;
        int mostCorrelated = -1;
        vector<float> feature = ts.getVecByFeature(mapFeatures.at(i));
        for (int j = i + 1; j < sizeOfFeatures; j++) {
            vector<float> comparedFeature = ts.getVecByFeature(mapFeatures.at(j));
            float p = std::fabs(pearson(&feature[0], &comparedFeature[0], (int) feature.size()));
            if (p > maxPearson) {
                maxPearson = p;
                mostCorrelated = j;
            }
        }
        if (mostCorrelated != -1) {
            // associate mapFeatures.at(i) and mapFeatures.at(j) as correlated feature
            setCorrelatedFeatures(mapFeatures.at(i), feature, mapFeatures.at(mostCorrelated),
                                  ts.getVecByFeature(mapFeatures.at(mostCorrelated)), maxPearson);
        }
    }
}

// detect the correlations and the to_report
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> reports;
    for (const correlatedFeatures &pair: *_matchedFeatures) {
        if (pair.radius == 0) {
            vector<float> feature1 = ts.getVecByFeature(pair.feature1);
            vector<float> feature2 = ts.getVecByFeature(pair.feature2);
            Point **points = fromVecToPoints(feature1, feature2);
            for (int i = 0; i < feature1.size(); i++) {
                float deviation = dev(*points[i], pair.lin_reg);
                if (deviation > pair.threshold) {
                    AnomalyReport report((pair.feature1 + "-" + pair.feature2), i + 1);
                    reports.push_back(report);
                }
            }
        }
    }
    return reports;
}

// get a points array from two vectors
Point **SimpleAnomalyDetector::fromVecToPoints(vector<float> vec1, vector<float> vec2) {
    auto **points = new Point *[vec1.size()];
    for (int i = 0; i < vec1.size(); i++) {
        points[i] = new Point(vec1[i], vec2[i]);
    }

    return points;
}

void SimpleAnomalyDetector::setThreshold(const float &threshold){
    _threshold = threshold;
}


// get the max deviation between points array and the line equation of the points
float getMaxDev(Point **points, Line linReg, int size) {
    float maxDev = 0;
    for (int i = 0; i < size; i++) {
        float tempDev = dev(*points[i], linReg);
        if (tempDev > maxDev) {
            maxDev = tempDev;
        }
    }
    return maxDev;
}

void SimpleAnomalyDetector::setCorrelatedFeatures(const string &f1, const vector<float> &feature1, const string &f2,
                                                  const vector<float> &feature2, float correlation) {
    if (correlation >= _threshold) {
        float x = 1.1;
        struct correlatedFeatures matched;
        Point **featuresToPoints = fromVecToPoints(feature1, feature2);
        matched.corrlation = correlation;
        matched.feature1 = f1;
        matched.feature2 = f2;
        matched.lin_reg = linear_reg(featuresToPoints, (int) feature1.size());
        matched.radius = 0;
        matched.center = {0, 0};
        matched.threshold = x * getMaxDev(featuresToPoints, matched.lin_reg, feature1.size());
        _matchedFeatures->push_back(matched);
    }

}

// getter for the normal model
vector<correlatedFeatures> SimpleAnomalyDetector::getNormalModel() {
    return *_matchedFeatures;
}

void SimpleAnomalyDetector::setMatchedFeatures(const vector<correlatedFeatures> &matched) {
    *_matchedFeatures = matched;
}

// constructor
SimpleAnomalyDetector::SimpleAnomalyDetector() {
    _matchedFeatures = new vector<correlatedFeatures>();
    _threshold = 0.9;
}


// destructor
SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    delete (_matchedFeatures);
}