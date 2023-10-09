// Copyright 2023 Baldovin-Razvan-Mihai-Marian @2amazing4me

#include <cmath>
#include <iostream>
#include <vector>

#define INF 1e9
#define HALF_HOUR 1800
#define HALF_HOUR_DOUBLE 1800.0

using namespace std;

/**
 * get_route_durations - Reads the answer.json file and stores the durations for
 * each route in between 2 locations
 *
 * @param route_matrix: The matrix that stores the durations for each route
 * every 30 minutes
 * @param iteration: The current time iteration (0 - current, 1 - +30 minutes, 2
 * - +1 hour, 3 - +1.5 hours)
 */
void get_route_durations(vector<vector<vector<int>>> &route_matrix,
						 int iteration);

/**
 * get_route - Recursive function that finds the shortest route
 *
 * @param route_matrix: The matrix that stores the durations for each route
 * every 30 minutes
 * @param nr_locations: The number of locations
 * @param route: The current route
 * @param current_location: The current location index
 * @param address_mask: Binary mask that stores 1 for each address that has been
 * visited and 0 for each address that hasn't been visited
 * @param current_time: The current time
 * @param min_time: The minimum time (shortest route)
 * @param min_route: The minimum route
 */
void get_route(vector<vector<vector<int>>> &route_matrix, int nr_locations,
			   vector<int> &route, int current_location, int address_mask,
			   int current_time, int &min_time, vector<int> &min_route);

/**
 * get_route_with_return - Recursive function that finds the shortest route that
 * also return to the starting point
 *
 * @param route_matrix: The matrix that stores the durations for each route
 * every 30 minutes
 * @param nr_locations: The number of locations
 * @param route: The current route
 * @param current_location: The current location index
 * @param address_mask: Binary mask that stores 1 for each address that has been
 * visited and 0 for each address that hasn't been visited
 * @param current_time: The current time
 * @param min_time: The minimum time (shortest route)
 * @param min_route: The minimum route
 */
void get_route_with_return(vector<vector<vector<int>>> &route_matrix,
						   int nr_locations, vector<int> &route,
						   int current_location, int address_mask,
						   int current_time, int &min_time,
						   vector<int> &min_route);