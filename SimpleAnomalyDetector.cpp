#include "AnomalyDetector.h"
#include "SimpleAmomalyDetector.h"

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
            float p = std::abs(pearson(&feature[0], &comparedFeature[0], (int) mapFeatures.at(j).size()));
            if ((p > maxPearson) && (p > threshold)) {
                maxPearson = p;
                mostCorrelated = j;
            }
            if (mostCorrelated != -1) {
                // associate mapFeatures.at(i) and mapFeatures.at(j) as correlated features
                setCorrelatedFeatures(feature, comparedFeature, p);
            }
        }
    }
}


// the detect parse
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {

}


vector<correlatedFeatures> SimpleAnomalyDetector::getNormalModel() {
    return matchedFeatures;
}


Point **SimpleAnomalyDetector::fromVecToPoints(vector<float> vec1, vector<float> vec2) {
    auto **points = new Point*[vec1.size()];
    for (int i = 0; i < vec1.size(); i++) {
        points[i]->x = vec1[i];
        points[i]->y = vec2[i];
    }
    return points;
}

float getMaxDev(Point **points, Line linReg, int size){
    float maxDev = 0;
    for (int i = 0; i < size; i++){
        float tempDev = dev(*points[i],linReg);
        if ( tempDev > maxDev){
            maxDev = tempDev;
        }
    }
    return maxDev;
}

void SimpleAnomalyDetector::setCorrelatedFeatures(const vector<float> &feature1, const vector<float> &feature2,
                                                  float corrlation) {
    float x = 1.1;
    correlatedFeatures matched;
    Point **featuresToPoints = fromVecToPoints(feature1,feature2);
    matched.corrlation = corrlation;
    matched.feature1.assign(feature1.begin(), feature1.end());
    matched.feature2.assign(feature2.begin(), feature2.end());
    matched.lin_reg = linear_reg(featuresToPoints, (int)feature1.size());
    matched.threshold = x * getMaxDev(featuresToPoints, matched.lin_reg, (int)feature1.size());
    matchedFeatures.push_back(matched);
}
