#include <stdio.h>
#include <mpi.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>


/**
 * This program will use a sequential dynamic TSP solution. Function is given S=unvisited cities, and s=source city, c=current city,
 * and must find the shortest path from c back to source through unvisited cities
 */


using namespace std;


struct city {
    float x;
    float y;
};

vector<city> cities;

int source;
int current;
vector<int> unvisited;
float** distances_array;

float getDist(int city1, int city2) {
    return distances_array[city1][city2];
}

float dynamicSolution() {
    if(unvisited.size() == 1) {
        return getDist(current, source);
    } else {
        // explore all unvisited, ignore source
        float store_current = current;
        int nextCity;
        float min_dist = 1000000000;
        for(int i = 0; i < unvisited.size(); i++) {
            // ignore source
            if(unvisited[i] == source) continue;
            // now do recursion

            // set the new current
            nextCity = unvisited[i];
            current = unvisited[i];

            // set the city as visited, then check, then set as unvisited again afterwards
            unvisited.erase(unvisited.begin() + i);
            float dist = getDist(store_current, nextCity) + dynamicSolution();
            unvisited.insert(unvisited.begin() + i, nextCity);
            if(dist < min_dist) min_dist = dist;
        }
        current = store_current;
        return min_dist;
    }
}

float startDynamicSolution() {

    unvisited.clear();
    for(unsigned i = 0; i < cities.size(); i++) {
        unvisited.push_back(i);
    }

    // start solving
    source = 0;
    current = 0;
    return dynamicSolution();
}




int main() {

    cout << "Project starting." << endl;

    // Open the file
    ifstream file;
    file.open("cities.data");
    if (!file.is_open()) return 1;

    // Read city data from file
    string x_pos;
    string y_pos;
    while(file >> x_pos) {
        file >> y_pos;

        city newCity;
        newCity.x = atof(x_pos.c_str());
        newCity.y = atof(y_pos.c_str());
        cities.push_back(newCity);
    }

    cout << "number of cities: " << cities.size() << endl;

    distances_array = new float*[cities.size()];
    for(int i = 0; i < cities.size(); i++) {
        distances_array[i] = new float[cities.size()];
    }

    for(int i = 0; i < cities.size(); i++) {
        for(int j = 0; j < cities.size(); j++) {
            float distance = sqrt((cities[i].x-cities[j].x)*(cities[i].x-cities[j].x) + (cities[i].y-cities[j].y)*(cities[i].y-cities[j].y));
            distances_array[i][j] = distance;
        }
    }

    float dist = startDynamicSolution();

    cout << "dist is " << dist << endl;

    return 0;
}