cmake_minimum_required(VERSION 3.20)
project(AP_Project)

set(CMAKE_CXX_STANDARD 11)

add_executable(AP_Project anomaly_detection_util.h anomaly_detection_util.cpp AnomalyDetector.h timeseries.cpp
        SimpleAnomalyDetector.h SimpleAnomalyDetector.cpp MainTrain.cpp timeseries.h minCircle.cpp minCircle.h
        HybridAnomalyDetector.cpp HybridAnomalyDetector.h)