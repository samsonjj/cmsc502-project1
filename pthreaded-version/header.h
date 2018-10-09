
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


