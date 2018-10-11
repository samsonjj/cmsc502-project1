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

void *findAndStoreSolution(void* v) {
    thread_vars* vars = (thread_vars*) v;
    solution s = startDynamicSolution(vars, vars->cities);
    vars->solutionArray[vars->i][vars->j] = s;
}

int main() {


    readInCities();



    sort(all_cities.begin(), all_cities.end(), compareCitiesByY);

    for(int i = 0; i < all_cities.size(); i++) {
        cout << "City at (" << setw(7) << all_cities[i].x << "," << setw(7) << all_cities[i].y << ")" << endl;
    }


    vector<vector<city>> rows;
    int chunk_size = floor(1.0 * all_cities.size() / GRID_LENGTH);
    int extra = all_cities.size() % GRID_LENGTH;
    for(int i = 0; i < GRID_LENGTH; i++) {
        vector<city>::iterator start = all_cities.begin() + chunk_size * i + min(i, extra);
        vector<city>::iterator end = all_cities.begin() + chunk_size * (i+1) + min(i+1, extra);
        vector<city> newVector(start, end);
        rows.push_back(newVector);
    }

    for(int i = 0; i < rows.size(); i++) {
        sort(rows[i].begin(), rows[i].end(), compareCitiesByX); 
    }

    vector<vector<vector<city>>> blocks(GRID_LENGTH);
    for(int i = 0; i < GRID_LENGTH; i++) {
        chunk_size = floor(1.0 * rows[i].size() / GRID_LENGTH);
        extra = rows[i].size() % GRID_LENGTH;
        for(int j = 0; j < GRID_LENGTH; j++) {
            // the extra part is to make sure we include all the elements, since chunk size is taken with a FLOOR operation...
            // witout extra, some elements may be cut out
            vector<city>::iterator start = rows[i].begin() + chunk_size * j + min(j, extra);
            vector<city>::iterator end = rows[i].begin() + chunk_size * (j+1) + min(j+1, extra);
            vector<city> newVector(start, end);
            blocks[i].push_back(newVector);
        }
    }

    cout << "number of rows: " << blocks.size() << endl;
    cout << "number of blocks in first row " << blocks[0].size() << endl;
    cout << "number of cities in first block of first row " << blocks[0][0].size() << endl;
    
    for(int i = 0; i < blocks.size(); i++) {
        for(int j = 0; j < blocks[i].size(); j++) {
            cout << blocks[i][j].size() << " ";
        }
        cout << endl;
    }

    thread_vars *vars;
    vector<float> tsp;
    vector<city> cities_by_id;
    solution **solutionArray = new solution*[blocks.size()];
    pthread_t **threadArray = new pthread_t*[GRID_LENGTH];
    int count = 0;
    for(int i = 0; i < blocks.size(); i++) {
        solutionArray[i] = new solution[blocks[i].size()];
        threadArray[i] = new pthread_t[GRID_LENGTH];
        for(int j = 0; j < blocks[i].size(); j++) {
            cout << i << "," << j << " | " << std::flush;
            for(int k = 0; k < blocks[i][j].size(); k++) {
                cout << blocks[i][j][k].x << " ";
                blocks[i][j][k].id = count;
                count++;
                cities_by_id.push_back(blocks[i][j][k]);
            }
            cout << endl; 

            vector<city> cities = blocks[i][j];
            thread_vars *vars = new thread_vars();
            vars->cities = cities;
            vars->solutionArray = solutionArray;
            vars->i = i;
            vars->j = j;
            pthread_create(&threadArray[i][j], NULL, findAndStoreSolution, (void *) vars);
            
        }
    }

    for(int i = 0; i < GRID_LENGTH; i++) {
        for(int j = 0; j < GRID_LENGTH; j++) {
            void *status;
            pthread_join(threadArray[i][j], &status);
        }
    }


    // use cities_by_id to go through cities
    // store what cities are visited
    // cycle through the 2 cities which are visited, but not fully connected, and find their closest non-visited city
    // then link with them, calculate distance and add to total
    int city_at_end_1 = 0;
    int city_at_end_2;
    
    for(int i = 0; i < GRID_LENGTH; i++) {
        for(int j = 0; j < GRID_LENGTH; j++) {
            


    for(int i = 0; i < blocks.size(); i++) {
        for(int j = 0; j < blocks[i].size(); j++) {
            
            cout << solutionArray[i][j].distance << ", " << solutionArray[i][j].last_city << ", " << solutionArray[i][j].first_city << endl;

        }
    }

    

    return 0;
}
