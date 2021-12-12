#include "minCircle.h"
#include "anomaly_detection_util.h"
#include <random>
#include <vector>
#include "math.h"
#include "algorithm"

// Function to return the euclidean distance between two points
float dist(const Point& a, const Point& b) {
    return sqrtf(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)));
}

// Function to check whether a point lies inside or on the boundaries of the circle
bool checkLocation(const Circle& c, const Point& p) {
    return dist(c.center, p) <= c.radius;
}


// The following two functions are the functions used
// To find the equation of the circle when three points are given.
// Helper method to get a circle defined by 3 points
Point getCircleCenter(float bx, float by, float cx, float cy) {
    float B = bx * bx + by * by;
    float C = cx * cx + cy * cy;
    float D = bx * cy - by * cx;
    return { (cy * B - by * C) / (2 * D), (bx * C - cx * B) / (2 * D) };
}

// Function to return a unique circle that intersects three points
Circle findCircle(const Point& A, const Point& B, const Point& C) {
    Point centerOfCircle = getCircleCenter(B.x - A.x, B.y - A.y, C.x - A.x, C.y - A.y);
    centerOfCircle.x += A.x;
    centerOfCircle.y += A.y;
    return { centerOfCircle, dist(centerOfCircle, A) };
}

// Function to return the smallest circle that intersects 2 points
Circle findCircle(const Point& A, const Point& B) {

    // Set the center to be the midpoint of A and B
    Point C = { (A.x + B.x) / 2, (A.y + B.y) / 2 };

    // Set the radius to be half the distance AB
    return { C, dist(A, B) / 2 };
}

// Function to check whether a circle encloses the given points
bool isValidCircle(const Circle& c, vector<Point>& points) {

    // Iterating through all the points to check whether the points lie inside the circle or not
    for (const Point& p : points) {
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
        return { { 0, 0 }, 0 };
    }
    else if (points.size() == 1) {
        return { points[0], 0 };
    }
    else if (points.size() == 2) {
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

// Returns the MEC using Welzl's algorithm
// Takes a set of input points P and a set R
// points on the circle boundary.
// n represents the number of points in P
// that are not yet processed.
Circle findMinCircleHelper(vector<Point> points, vector<Point> tmpVec, int size) {

    // Base case when all points processed or |R| = 3
    if (size == 0 || tmpVec.size() == 3) {
        return basicMinCircle(tmpVec);
    }
    int index = rand() % size;
    Point p = points[index];

    // Put the picked point at the end of P since it's more efficient than deleting from the middle of the vector
    swap(points[index], points[size - 1]);

    // Get the MEC circle d from the  set of points P - {p}
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

Circle findMinCircle(Point** points, size_t size){
    vector<Point> pointsCopy;
    for (int i = 0; i < size; i++){
        pointsCopy.push_back(*points[i]);
    }
    random_shuffle(pointsCopy.begin(), pointsCopy.end());
    return findMinCircleHelper(pointsCopy, {}, pointsCopy.size());
}
