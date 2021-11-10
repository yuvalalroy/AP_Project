#include "timeseries.h"
#include "SimpleAnomalyDetector.h"

// the previous parse
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    vector<string> mapFeatures = ts.getFeatures();
    unsigned int sizeOfFeatures = mapFeatures.size();
    float threshold = 0.9;

    for (int i = 0; i < sizeOfFeatures; i++) {
        float maxPearson = 0;
        int mostCorrelated = -1;
        vector<float> feature = ts.getVecByFeature(mapFeatures.at(i));
        for (int j = i + 1; j < sizeOfFeatures; j++) {
            vector<float> comparedFeature = ts.getVecByFeature(mapFeatures.at(j));
            float p = std::abs(pearson(&feature[0], &comparedFeature[0], (int) mapFeatures.size()));
            if ((p > maxPearson) && (p > threshold)) {
                maxPearson = p;
                mostCorrelated = j;
                if (mostCorrelated != -1) {
                    // associate mapFeatures.at(i) and mapFeatures.at(j) as correlated features
                    setCorrelatedFeatures(mapFeatures.at(i), feature, mapFeatures.at(j), comparedFeature, p);
                }
            }
        }
    }
}

// the detect parse
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> reports;
    for (const correlatedFeatures &pair: *_matchedFeatures) {
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
    return reports;
}

vector<correlatedFeatures> SimpleAnomalyDetector::getNormalModel() {
    return *_matchedFeatures;
}


Point **SimpleAnomalyDetector::fromVecToPoints(vector<float> vec1, vector<float> vec2) {
    auto **points = new Point *[vec1.size()];
    for (int i = 0; i < vec1.size(); i++) {
        points[i] = new Point(vec1[i],vec2[i]);
    }
    return points;
}

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
    float x = 1.1;
    correlatedFeatures matched;
    Point **featuresToPoints = fromVecToPoints(feature1, feature2);
    matched.corrlation = correlation;
    matched.feature1 = f1;
    matched.feature2 = f2;
    matched.lin_reg = linear_reg(featuresToPoints, (int) feature1.size());
    matched.threshold = x * getMaxDev(featuresToPoints, matched.lin_reg, (int) feature1.size());
    _matchedFeatures->push_back(matched);
}

SimpleAnomalyDetector::SimpleAnomalyDetector(){
    _matchedFeatures = new vector<correlatedFeatures>();
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    delete(_matchedFeatures);
}