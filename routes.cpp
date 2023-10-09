// Copyright 2023 Baldovin-Razvan-Mihai-Marian @2amazing4me

#ifndef ROUTES_H
#define ROUTES_H

#include "routes.h"

void get_route_durations(vector<vector<vector<int>>> &route_matrix,
						 int iteration)
{
	FILE *routes = fopen("answer.json", "r");
	if (routes == NULL) {
		cout << "Error opening file" << endl;
		exit(1);
	}

	int origin, destination, duration;
	while (feof(routes) == 0) {
		fscanf(routes, "%d %d %d", &origin, &destination, &duration);
		route_matrix[iteration][origin][destination] = duration;
	}
}

void get_route(vector<vector<vector<int>>> &route_matrix, int nr_locations,
			   vector<int> &route, int current_location, int address_mask,
			   int current_time, int &min_time, vector<int> &min_route)
{
	// If all addresses have been visited, return
	if (address_mask == (1 << nr_locations) - 1) {
		// Print the route and the time it takes for it
		cout << "current_time: " << current_time << endl;
		for (int i = 0; i < route.size(); ++i)
			cout << route[i] << " ";

		// Update the route and the time if the current route is faster
		if (current_time < min_time) {
			cout << "e minim";
			min_time = current_time;
			min_route = route;
		}
		cout << endl;
		return;
	}

	// Get the current time iteration
	int iteration = min(3, current_time / HALF_HOUR);

	// For each address that has not been visited yet
	for (int i = 0; i < nr_locations; ++i) {
		if ((address_mask & (1 << i)) == 0 && i != current_location) {
			// Add the address to the route
			route.push_back(i);

			// Get the time to the next location
			int time_to_next_location =
				route_matrix[iteration][current_location][i];

			// If the current time is in between 2 time iterations, calculate
			// the time to the next location using a weighted average
			if (iteration < 3) {
				double weight = (current_time % HALF_HOUR) / HALF_HOUR_DOUBLE;

				time_to_next_location =
					time_to_next_location * (1.0 - weight) +
					route_matrix[iteration + 1][current_location][i] * weight;
			}

			// Don't check the route if it's longer than the minimum time
			if (current_time + time_to_next_location < min_time)
				// Recursively call the function for the next address
				get_route(route_matrix, nr_locations, route, i,
						  address_mask | (1 << i),
						  current_time + time_to_next_location, min_time,
						  min_route);

			// Remove the address from the route
			route.pop_back();
		}
	}
}

void get_route_with_return(vector<vector<vector<int>>> &route_matrix,
						   int nr_locations, vector<int> &route,
						   int current_location, int address_mask,
						   int current_time, int &min_time,
						   vector<int> &min_route)
{
	// Get the current time iteration
	int iteration = min(3, current_time / HALF_HOUR);

	// If all addresses have been visited, return
	if (address_mask == (1 << nr_locations) - 1) {
		int time_to_next_location =
			route_matrix[iteration][current_location][0];

		// If the current time is in between 2 time iterations, calculate
		// the time to the next location using a weighted average
		if (iteration < 3) {
			double weight = (current_time % HALF_HOUR) / HALF_HOUR_DOUBLE;

			time_to_next_location =
				time_to_next_location * (1.0 - weight) +
				route_matrix[iteration + 1][current_location][0] * weight;
		}

		current_time += time_to_next_location;
		route.push_back(0);

		// Print the route and the time it takes for it
		cout << "current_time: " << current_time << endl;
		for (int i = 0; i < route.size(); ++i)
			cout << route[i] << " ";

		// Update the route and the time if the current route is faster
		if (current_time < min_time) {
			cout << "e minim";
			min_time = current_time;
			min_route = route;
		}
		cout << endl;
		route.pop_back();

		return;
	}

	// For each address that has not been visited yet
	for (int i = 0; i < nr_locations; ++i) {
		if ((address_mask & (1 << i)) == 0 && i != current_location) {
			// Add the address to the route
			route.push_back(i);

			// Get the time to the next location
			int time_to_next_location =
				route_matrix[iteration][current_location][i];

			// If the current time is in between 2 time iterations, calculate
			// the time to the next location using a weighted average
			if (iteration < 3) {
				double weight = (current_time % HALF_HOUR) / HALF_HOUR_DOUBLE;

				time_to_next_location =
					time_to_next_location * (1.0 - weight) +
					route_matrix[iteration + 1][current_location][i] * weight;
			}

			// Don't check the route if it's longer than the minimum time
			if (current_time + time_to_next_location < min_time)
				// Recursively call the function for the next address
				get_route_with_return(route_matrix, nr_locations, route, i,
									  address_mask | (1 << i),
									  current_time + time_to_next_location,
									  min_time, min_route);

			// Remove the address from the route
			route.pop_back();
		}
	}
}

#endif // ROUTES_H