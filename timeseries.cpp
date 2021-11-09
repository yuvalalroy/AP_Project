//
// Created by yuvalalroy on 11/1/21.
//


#include <sstream>
#include <fstream>
#include "timeseries.h"


std::vector<float> timeseries::getVecByFeature(const std::string& feature) const {
    return _dataMap.at(feature);
}


void timeseries::setMapKeys(const std::string &keys) {
    std::string feature;
    std::stringstream stream(keys);

    while (getline(stream, feature, ',')) {
        _dataMap.insert(std::pair<std::string, std::vector<float>>(feature, {}));
    }
}


void timeseries::setMapValues(const std::string &values) {
    std::map<std::string, std::vector<float>>::iterator iter;
    iter = _dataMap.begin();
    std::string val;
    std::stringstream stream(values);
    while (getline(stream, val, ',')) {
        iter->second.push_back(stof(val));
        iter++;
    }
}


void timeseries::chargeCSVToMap(const char *CSVFile) {
    std::ifstream file;
    file.open(CSVFile);
    std::string line;

    // read the first line - keys
    std::getline(file, line);
    setMapKeys(line);

    // read the rest of the file - values
    while (getline(file, line)) {
        setMapValues(line);
    }
}


std::vector<std::string> timeseries::getFeatures() const {
    std::vector<std::string> features;
    for (const auto & iter : _dataMap) {
        features.push_back(iter.first);
    }
    return features;
}