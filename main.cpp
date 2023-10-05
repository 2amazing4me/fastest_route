// Copyright 2023 Baldovin-Razvan-Mihai-Marian @2amazing4me

#include <iostream>
#include <vector>

#include "API_handler.h"
#include "routes.h"

using namespace std;

int main()
{
	int nr_locations;
	vector<string> addresses;
	vector<vector<vector<int>>> route_matrix;
	prepare_and_call_API(nr_locations, addresses, route_matrix);

	// Print all the route matrices
	for (int k = 0; k < 4; ++k) {
		for (int i = 0; i < nr_locations; ++i) {
			for (int j = 0; j < nr_locations; ++j)
				cout << setw(7) << route_matrix[k][i][j] << " ";
			cout << endl;
		}
		cout << endl;
	}

	// Start generating the fastest route
	vector<int> route = {0}, min_route;
	int address_mask = 1;
	int min_time = INF;
	get_route(route_matrix, nr_locations, route, 0, address_mask, 0, min_time,
			  min_route);

	// Print the fastest route
	for (int i = 0; i < min_route.size(); ++i)
		cout << min_route[i] << ". " << addresses[min_route[i]] << endl;

	// Calculates and prints the time for the fastest route
	int hours = min_time / 3600;
	int minutes = (min_time % 3600) / 60;
	int seconds = min_time % 60;
	cout << "Time: " << hours << " hours, " << minutes << " minutes, "
		 << seconds << " seconds" << endl;

	redirect_to_web(min_route, addresses);

	return 0;
}