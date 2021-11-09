//
// Created by amit on 08/11/2021.
//

#ifndef AP_PROJECT_TIMESERIES_H
#define AP_PROJECT_TIMESERIES_H

#include <string>
#include <vector>
#include <map>

using std::vector;
using std::string;

class timeseries {
    std::map<string, vector<float>> _dataMap;
public:
    explicit timeseries(const char* CSVFile){
        chargeCSVToMap(CSVFile);
    }
    void setMapKeys(const string& keys);
    void setMapValues(const string& values);
    void chargeCSVToMap(const char* CSVFile);
    vector<string> getFeatures() const;
    vector<float> getVecByFeature(const string &feature) const;
};



#endif //AP_PROJECT_TIMESERIES_H
