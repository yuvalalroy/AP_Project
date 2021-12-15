

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
#include <vector>

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);
    virtual void setCorrelatedFeatures(const string &f1, const vector<float> &feature1, const string &f2,
                                                              const vector<float> &feature2, float correlation);

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
