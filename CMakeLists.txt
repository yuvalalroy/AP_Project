cmake_minimum_required(VERSION 3.20)
project(AP_Project)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-pthread")

add_executable(AP_Project anomaly_detection_util.h anomaly_detection_util.cpp AnomalyDetector.h timeseries.cpp
        SimpleAnomalyDetector.h SimpleAnomalyDetector.cpp MainTrain.cpp timeseries.h minCircle.cpp minCircle.h
        HybridAnomalyDetector.cpp HybridAnomalyDetector.h CLI.cpp CLI.h commands.h Server.cpp Server.h)