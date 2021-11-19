// Amit Paz ID:319003455
// Yuval Alroy ID:315789461

#ifndef AP_PROJECT_TIMESERIES_H
#define AP_PROJECT_TIMESERIES_H

#include <string>
#include <vector>
#include <map>

using std::vector;
using std::string;

class TimeSeries {
    std::map<string, vector<float>> _dataMap;
public:

    // constructor
    explicit TimeSeries(const char* CSVFile){
        chargeCSVToMap(CSVFile);
    }

    // set the map keys by the CSV file
    void setMapKeys(const string& keys);

    // set the map values by the CSV file
    void setMapValues(const string& values);

    // main function that charges the CSV file into our map
    void chargeCSVToMap(const char* CSVFile);

    // getter for the features (keys in the map)
    vector<string> getFeatures() const;

    // getter for the vectors by feature (get value by key)
    vector<float> getVecByFeature(const string &feature) const;
};

#endif //AP_PROJECT_TIMESERIES_H
