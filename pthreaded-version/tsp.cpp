
#include "header.h"

using namespace std;

int computeDistanceArray(thread_vars *vars, std::vector<city> cities) {
    vars->distance_array = new float*[cities.size()];
    for(int i = 0; i < cities.size(); i++) {
        vars->distance_array[i] = new float[cities.size()];
    }

    for(int i = 0; i < cities.size(); i++) {
        for(int j = 0; j < cities.size(); j++) {
            float distance = calcDistance(cities[i].x, cities[i].y, cities[j].x, cities[j].y);
            //cout << "distance: " << distance << "," << cities[i].x << "," << cities[i].y << "," << cities[j].x << "," << cities[j].y << endl;
            vars->distance_array[i][j] = distance;
        }
    }
}

solution dynamicSolution(thread_vars *vars) {
    if(vars->unvisited.size() == 0) {
        solution sol;
        sol.distance = 0;
        sol.last_city = vars->cities[vars->currentPath.back()].id;
        return sol;
    } else {
        // explore all unvisited, ignore source
        float min_dist = std::numeric_limits<float>::max();
        int min_last_city;
        for(int i = 0; i < vars->unvisited.size(); i++) {

            // set the new current
            vars->currentPath.push_back(vars->unvisited[i]);

            // set the city as visited, then check, then set as unvisited again afterwards
            vars->unvisited.erase(vars->unvisited.begin() + i);
            solution dSol = dynamicSolution(vars);
            vars->unvisited.insert(vars->unvisited.begin() + i, vars->currentPath.back());
            vars->currentPath.pop_back();

            float dist = vars->distance_array[vars->currentPath.back()][vars->unvisited[i]] + dSol.distance;
            /*if(dist > 1000000 && vars->unvisited.size() < 2) {
                cout << vars->currentPath.back() << ", " << vars->unvisited[i] << ", " << vars->distance_array[vars->currentPath.back()][vars->unvisited[i]] << ", " << vars->cities[vars->unvisited[i]].x << endl;
            }*/
            if(dist < min_dist) {
                min_dist = dist;
                min_last_city = dSol.last_city;
            }
        }
        solution sol;
        sol.distance = min_dist;
        sol.last_city = min_last_city;

        return sol;
    }
}

solution startDynamicSolution(thread_vars *vars, std::vector<city> cities) {

    vars->unvisited.clear();

    // start at 1 since 0 will be source, and will count as visited
    for(unsigned i = 1; i < cities.size(); i++) {
        vars->unvisited.push_back(i);
    }
    vars->currentPath.push_back(0);
    
    computeDistanceArray(vars, cities);
    printMatrix(vars->distance_array, cities.size(), cities.size()); 

    solution theSolution = dynamicSolution(vars);
    theSolution.first_city = vars->cities[0].id;
    return theSolution;
}
