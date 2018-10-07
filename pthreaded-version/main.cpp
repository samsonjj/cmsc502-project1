#include <stdio.h>
#include <mpi.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include <limits>

#include<bits/stdc++.h>

/**
 * This program will use a sequential dynamic TSP solution. Function is given S=unvisited cities, and s=source city, c=current city,
 * and must find the shortest path from c back to source through unvisited cities
 */


using namespace std;

const int GRID_LENGTH = 10;

struct city {
    float x;
    float y;
};

// algorithm variables
int source;
int current;
vector<city> cities;
vector<int> unvisited;

// distances array
float** distances_array;

void printMatrix(float** matrix, int r, int c) {

    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            cout << matrix[r][c] << " ";
        }
        cout << endl;
    }
}

float getDist(int city1, int city2) {
    return distances_array[city1][city2];
}

float dynamicSolution() {
    if(unvisited.size() == 0) {
        return getDist(current, source);
    } else {
        // explore all unvisited, ignore source
        float store_current = current;
        int nextCity;
        float min_dist = numeric_limits<float>::max();
        for(int i = 0; i < unvisited.size(); i++) {

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

    // start at 1 since 0 will be source, and will count as visited
    for(unsigned i = 1; i < cities.size(); i++) {
        unvisited.push_back(i);
    }

    // start solving
    source = 0;
    current = 0;
    return dynamicSolution();
}

int readInCities() {
    ifstream file;
    file.open("cities.data");
    if (!file.is_open()) {
        return 1;
    }

    string x_pos;
    string y_pos;
    while(file >> x_pos >> y_pos) {
        city newCity;
        newCity.x = atof(x_pos.c_str());
        newCity.y = atof(y_pos.c_str());
        cities.push_back(newCity);
    }
}

int computeDistancesArray() {
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
}

bool compareCitiesByX(city a, city b) {
    return a.x < b.x;
}

bool compareCitiesByY(city a, city b) {
    return a.y < b.y;
}

int main() {


    readInCities();



    sort(cities.begin(), cities.end(), compareCitiesByY);

    for(int i = 0; i < cities.size(); i++) {
        cout << "City at (" << cities[i].x << "," << cities[i].y << ")" << endl;
    }


    vector<vector<city>> rows;
    for(int i = 0; i < GRID_LENGTH; i++) {
        int chunk_size = ceil(1.0 * cities.size() / GRID_LENGTH);
        vector<city>::iterator start = cities.begin() + chunk_size * i;
        vector<city>::iterator end = min(cities.begin() + chunk_size * (i+1), cities.end());
        vector<city> newVector(start, end);
        cout << "start: " << &(*start)<< ", end: " << &(*end) << ", size: " << newVector.size() << endl;
        rows.push_back(newVector);
    }

    cout << "rows has " << rows.size() << endl;
    cout << "first of rows has " << rows[0].size() << endl;

    vector<vector<vector<city>>> blocks(GRID_LENGTH);
    for(int i = 0; i < GRID_LENGTH; i++) {
        for(int j = 0; j < GRID_LENGTH; j++) {
            int chunk_size = ceil(1.0 * rows[i].size() / GRID_LENGTH);
            vector<city>::iterator start = rows[i].begin() + chunk_size * j;
            vector<city>::iterator end = min(rows[i].begin() + chunk_size * (j+1), rows[i].end());
            vector<city> newVector(start, end);
            cout << newVector.size();
            blocks[i].push_back(newVector);
        }
    }

    cout << "number of rows: " << blocks.size() << endl;
    cout << "number of blocks in first row " << blocks[0].size() << endl;
    cout << "number of cities in first block of first row " << blocks[0][0].size() << endl;

    computeDistancesArray();

    //float dist = startDynamicSolution();

    //cout << "minimum distance is " << dist << endl;

    return 0;
}
