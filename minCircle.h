#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;

// ------------ DO NOT CHANGE -----------

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

Circle findMinCircle(Point** points,size_t size);

bool checkLocation(const Circle& c, const Point& p);

Circle findCircle(const Point& A, const Point& B, const Point& C);

Circle findCircle(const Point& A, const Point& B);

bool isValidCircle(const Circle& c, const vector<Point>& points);

float dist(const Point& a, const Point& b);


#endif /* MINCIRCLE_H_ */