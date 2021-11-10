//
// Created by yuvalalroy on 11/1/21.
//

#include "anomaly_detection_util.h"
#include <cmath>

// returns the expected value of the values in the array
float getExpectedValue(float *arr, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return (1 / (float) size) * sum;
}

// returns the variance of X and Y
float var(float *x, int size) {
    float exVal = getExpectedValue(x, size);
    float doubleSum = 0;
    for (int i = 0; i < size; i++) {
        doubleSum += powf(x[i], 2);
    }

    return (doubleSum / (float) size) - (exVal * exVal);
}

// returns the covariance of X and Y
float cov(float *x, float *y, int size) {
    float exValX = getExpectedValue(x, size);
    float exValY = getExpectedValue(y, size);
    float xy[size];
    for (int i = 0; i < size; i++) {
        xy[i] = x[i] * y[i];
    }
    float exValXY = getExpectedValue(xy, size);
    return (exValXY - (exValX * exValY));
}

// returns the Pearson corrlation coefficient of X and Y
float pearson(float *x, float *y, int size) {
    float sigmaX = sqrtf(var(x, size));
    float sigmaY = sqrtf(var(y, size));

    return cov(x, y, size) / (sigmaX * sigmaY);
}

// performs a linear regression and return s the line equation
Line linear_reg(Point **points, int size) {
    float arrX[size];
    float arrY[size];

    for (int i = 0; i < size; i++) {
        arrX[i] = points[i]->x;
        arrY[i] = points[i]->y;
    }

    float a = cov(arrX, arrY, size) / var(arrX, size);
    float b = getExpectedValue(arrY, size) - (a * getExpectedValue(arrX, size));
    return {a, b};
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size) {
    Line l = linear_reg(points, size);
    return dev(p, l);
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
    float fx = l.f(p.x);
    float diff = fx - p.y;
    return diff >= 0 ? diff : -diff;
}