#include "minCircle.h"
#include "anomaly_detection_util.h"
#include <random>
#include <vector>
#include "math.h"
#include "algorithm"

// Function to return the euclidean distance between two points
float dist(const Point &a, const Point &b) {
    return sqrtf(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)));
}

// Function to check whether a point lies inside or on the boundaries of the circle
bool checkLocation(const Circle &c, const Point &p) {
    return dist(c.center, p) <= c.radius;
}


// Function to return a unique circle that intersects three points
Circle findCircle(const Point &A, const Point &B, const Point &C) {

    Point middleAB((A.x + B.x) / 2, (A.y + B.y) / 2); // middle point of line AB
    float gradientAB = (B.y - A.y) / (B.x - A.x); // the gradient of AB

//     the perpendicular gradient of AB. perpGradient equation is:
//     y - middleAB.y = perpGradientAB * (x - middleAB.x) ==> y = perpGradientAB * (x - middleAB.x) + middleAB.y
    float perpGradientAB = -1 / gradientAB;


    Point middleBC((B.x + C.x) / 2, (B.y + C.y) / 2); // middle point of line BC
    float gradientBC = (C.y - B.y) / (C.x - B.x); // the gradient of BC

//     the perpendicular gradient of BC. perpGradient equation is:
//     y - middleBC.y = perpGradientBC * (x - middleBC.x) ==> y = perpGradientBC * (x - middleBC.x) + middleBC.y
    float perpGradientBC = -1 / gradientBC;


    float x = (-perpGradientBC * middleBC.x + middleBC.y + perpGradientAB * middleAB.x - middleAB.y) /
              (perpGradientAB - perpGradientBC);
    float y = perpGradientAB * (x - middleAB.x) + middleAB.y;
    Point centerOfCircle(x, y);
    float R = dist(centerOfCircle, A);

    return {centerOfCircle, R};
}

// Function to return the smallest circle that intersects 2 points
Circle findCircle(const Point &A, const Point &B) {

    // Set the center to be the midpoint of A and B
    Point C = {(A.x + B.x) / 2, (A.y + B.y) / 2};

    // Set the radius to be half the distance AB
    return {C, dist(A, B) / 2};
}

// Function to check whether a circle encloses the given points
bool isValidCircle(const Circle &c, vector<Point> &points) {

    // Iterating through all the points to check whether the points lie inside the circle or not
    for (const Point &p: points) {
        if (!checkLocation(c, p))
            return false;
    }
    return true;
}

// Function to return the minimum enclosing
// circle for N <= 3
Circle basicMinCircle(vector<Point> points) {

//    assert(points.size() <= 3);
    if (points.empty()) {
        return {{0, 0}, 0};
    } else if (points.size() == 1) {
        return {points[0], 0};
    } else if (points.size() == 2) {
        return findCircle(points[0], points[1]);
    }

    // To check if MEC can be determined by 2 points only
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {

            Circle c = findCircle(points[i], points[j]);

            if (isValidCircle(c, points))
                return c;
        }
    }
    return findCircle(points[0], points[1], points[2]);
}

// Returns the MEC using Welzl's algorithm: takes a set of input points and a set tmpVec points on the circle boundary.
// size represents the number of points in 'points' that are not yet processed.
Circle findMinCircleHelper(Point** points, vector<Point> tmpVec, size_t size) {

    // Base case when all points processed or |tmpVec| = 3
    if (size == 0 || tmpVec.size() == 3) {
        return basicMinCircle(tmpVec);
    }
    int index = rand() % size;
    Point p(points[index]->x, points[index]->y);

    // Put the picked point at the end of points since its more efficient than deleting from the middle of the vector
    swap(points[index], points[size - 1]);

    // Get the MEC circle d from the  set of points 'points' - {p}
    Circle d = findMinCircleHelper(points, tmpVec, size - 1);

    // If d contains p, return d
    if (checkLocation(d, p)) {
        return d;
    }

    // Otherwise, must be on the boundary of the MEC
    tmpVec.push_back(p);

    // Return the MEC for P - {p} and R U {p}
    return findMinCircleHelper(points, tmpVec, size - 1);
}


Circle findMinCircle(Point **points, size_t size) {
    return findMinCircleHelper(points, {}, size);
}