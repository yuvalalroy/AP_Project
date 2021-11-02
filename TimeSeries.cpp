//
// Created by yuvalalroy on 11/1/21.
//

#include "AnomalyDetector.h"

void TimeSeries::setMapKeys(std::string keys) {
    std::string category;
    std::stringstream stream(keys);

    while(getline(stream, category, ',')) {
        _dataMap.insert(std::pair<std::string, std::vector<float>>(category, {}));
    }
}

void TimeSeries::setMapValues(std::string values) {
    std::string val;
    std::stringstream stream(values);

    while(getline(stream, val, ',')) {

    }
}

void TimeSeries::chargeCSVToMap(std::istream &str) {
    std::string keys;
    std::getline(str, keys);
    setMapKeys(keys);




}