#include "minCircle.h"
#include "anomaly_detection_util.h"
#include "HybridAnomalyDetector.h"


vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> reports;
    vector<AnomalyReport> maxCorReports;
    vector<AnomalyReport> minCorReports;

    for (const correlatedFeatures &pair: *_matchedFeatures) {
        maxCorReports = SimpleAnomalyDetector::detect(ts);
        if (pair.radius != 0) {
            vector<float> feature1 = ts.getVecByFeature(pair.feature1);
            vector<float> feature2 = ts.getVecByFeature(pair.feature2);
            Point **points = fromVecToPoints(feature1, feature2);
            for (int i = 0; i < feature1.size(); i++) {
                bool isValid = checkLocation({pair.center, pair.radius}, *points[i]);
                if (!isValid) {
                    AnomalyReport report((pair.feature1 + "-" + pair.feature2), i + 1);
                    minCorReports.push_back(report);
                }
            }
        }
    }

    for ( const AnomalyReport& report : minCorReports){ reports.push_back(report); }

    for ( const AnomalyReport& report : maxCorReports){ reports.push_back(report); }

    return reports;
}

void HybridAnomalyDetector::setCorrelatedFeatures(const string &f1, const vector<float> &feature1,
                                                          const string &f2, const vector<float> &feature2, float correlation) {
    SimpleAnomalyDetector::setCorrelatedFeatures(f1, feature1, f2, feature2, correlation);
    if ((correlation > 0.5) && (correlation < _threshold)){
        float x = 1.1;
        struct correlatedFeatures matched;
        Point **featuresToPoints = fromVecToPoints(feature1, feature2);
        Circle minCircle = findMinCircle(featuresToPoints, feature1.size());

        matched.corrlation = correlation;
        matched.feature1 = f1;
        matched.feature2 = f2;
        matched.lin_reg = {0,0};
        matched.radius =  x * minCircle.radius;
        matched.center = minCircle.center;
        matched.threshold = 0;
        _matchedFeatures->push_back(matched);
    }

}

HybridAnomalyDetector::HybridAnomalyDetector() = default;

HybridAnomalyDetector::~HybridAnomalyDetector() = default;
