#include "tsp.cpp"

/**
 * This program will use a sequential dynamic TSP solution. Function is given S=unvisited cities, and s=source city, c=current city,
 * and must find the shortest path from c back to source through unvisited cities
 */


using namespace std;

const int GRID_LENGTH = 10;

// distances array
vector<city> all_cities;

void printMatrix(float** matrix, int r, int c) {

    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            cout << matrix[r][c] << " ";
        }
        cout << endl;
    }
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
        all_cities.push_back(newCity);
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



    sort(all_cities.begin(), all_cities.end(), compareCitiesByY);

    for(int i = 0; i < all_cities.size(); i++) {
        cout << "City at (" << all_cities[i].x << "," << all_cities[i].y << ")" << endl;
    }


    vector<vector<city>> rows;
    for(int i = 0; i < GRID_LENGTH; i++) {
        int chunk_size = ceil(1.0 * all_cities.size() / GRID_LENGTH);
        vector<city>::iterator start = all_cities.begin() + chunk_size * i;
        vector<city>::iterator end = min(all_cities.begin() + chunk_size * (i+1), all_cities.end());
        vector<city> newVector(start, end);
        cout << "start: " << &(*start)<< ", end: " << &(*end) << ", size: " << newVector.size() << endl;
        rows.push_back(newVector);
    }

    cout << "rows has " << rows.size() << endl;
    cout << "first of rows has " << rows[0].size() << endl;

    for(int i = 0; i < rows.size(); i++) {
        sort(rows[i].begin(), rows[i].end(), compareCitiesByX); 
    }

    cout << "after sorting" << endl;


    vector<vector<vector<city>>> blocks(GRID_LENGTH);
    for(int i = 0; i < GRID_LENGTH; i++) {
        for(int j = 0; j < GRID_LENGTH; j++) {
            cout << "beginning of loop" << endl;
            int chunk_size = ceil(1.0 * rows[i].size() / GRID_LENGTH);
            vector<city>::iterator start = rows[i].begin() + chunk_size * j;
            vector<city>::iterator end = min(rows[i].begin() + chunk_size * (j+1), rows[i].end());
            cout << "something1" << endl;
            vector<city> newVector(start, end);
            cout << newVector.size() << endl;
            blocks[i].push_back(newVector);
        }
    }

    cout << "number of rows: " << blocks.size() << endl;
    cout << "number of blocks in first row " << blocks[0].size() << endl;
    cout << "number of cities in first block of first row " << blocks[0][0].size() << endl;


    vector<float> tsp;

    for(int i = 0; i < blocks.size(); i++) {
        for(int j = 0; j < blocks[i].size(); j++) {
            cout << i << "," << j << " | ";
            for(int k = 0; k < blocks[i][j].size(); k++) {
                cout << blocks[i][j][k].x << " ";
            }
            cout << endl;
            vector<city> cities = blocks[i][j];
            //float dist = startDynamicSolution();
            thread_vars *vars;
            cout << startDynamicSolution(vars, cities) << endl;
        }
    }

    return 0;
}
