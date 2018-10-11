
#include <stdio.h>
//#include <mpi.h>

#include <vector>
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <pthread.h>

#include<bits/stdc++.h>


struct city {
    float x;
    float y;
    int id;
};

struct solution {
    int first_city;
    int last_city;
    float distance;
    bool visited;

    // used in stitching
    int visited_cities;
};

struct thread_vars {
    int source;
    int current;
    int i;
    int j;
    std::vector<int> unvisited;
    float** distance_array;
    solution **solutionArray;
    std::vector<city> cities;
    std::vector<int> city_ids;
};

int NONE = 0;
int FIRST= 1;
int LAST = 2;
int BOTH = 3;

float calcDistance(float x1, float y1, float x2, float y2) {
    sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y2-y2));
}
