
#include "header.h"


int computeDistanceArray(thread_vars *vars, std::vector<city> cities) {
    vars->distance_array = new float*[cities.size()];
    for(int i = 0; i < cities.size(); i++) {
        vars->distance_array[i] = new float[cities.size()];
    }

    for(int i = 0; i < cities.size(); i++) {
        for(int j = 0; j < cities.size(); j++) {
            float distance = sqrt((cities[i].x-cities[j].x)*(cities[i].x-cities[j].x) + (cities[i].y-cities[j].y)*(cities[i].y-cities[j].y));
            vars->distance_array[i][j] = distance;
        }
    }
}

float dynamicSolution(thread_vars *vars) {
    if(vars->unvisited.size() == 0) {
        return vars->distance_array[vars->current][vars->source];
    } else {
        // explore all unvisited, ignore source
        float store_current = vars->current;
        int nextCity;
        float min_dist = std::numeric_limits<float>::max();
        for(int i = 0; i < vars->unvisited.size(); i++) {

            // set the new current
            nextCity = vars->unvisited[i];
            vars->current = vars->unvisited[i];

            // set the city as visited, then check, then set as unvisited again afterwards
            vars->unvisited.erase(vars->unvisited.begin() + i);
            float dist = vars->distance_array[vars->current][vars->source] + dynamicSolution(vars);
            vars->unvisited.insert(vars->unvisited.begin() + i, nextCity);
            if(dist < min_dist) min_dist = dist;
        }
        vars->current = store_current;
        return min_dist;
    }
}

float startDynamicSolution(thread_vars *vars, std::vector<city> cities) {

    vars->unvisited.clear();

    // start at 1 since 0 will be source, and will count as visited
    for(unsigned i = 1; i < cities.size(); i++) {
        vars->unvisited.push_back(i);
    }

    computeDistanceArray(vars, cities);

    // start solving
    vars->source = 0;
    vars->current = 0;
    return dynamicSolution(vars);
}
