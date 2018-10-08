
#include <stdio.h>
//#include <mpi.h>

#include <vector>
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>

#include<bits/stdc++.h>


struct city {
    float x;
    float y;
};

struct thread_vars {
    int source;
    int current;
    std::vector<int> unvisited;
    float** distance_array;
};

struct solution {
    int last_city;
    float distance;
};
