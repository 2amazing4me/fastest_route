// Copyright 2023 Baldovin-Razvan-Mihai-Marian @2amazing4me

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

/**
 * get_addresses - Gets the addresses from the user
 * TODO: Should be done automatically later on
 *
 * @param nr_locations: The number of locations
 * @param addresses: The vector that stores the addresses
 */
void get_addresses(int &nr_locations, vector<string> &addresses);

/**
 * getJSONPayload - Generates the JSON payload for the curl command
 *
 * @param addresses: The vector that stores the addresses
 * @param nr_locations: The number of locations
 * @param add_time: Adds 30 minutes to the departure time for better routes
 *
 * Return: JSON payload
 */
string getJSONPayload(vector<string> &addresses, int nr_locations,
					  int add_time);

/**
 * prepare_and_call_API - Prepares the curl command and calls the API
 *
 * @param nr_locations: The number of locations
 * @param addresses: The vector that stores the addresses
 * @param route_matrix: The matrix that stores the durations for each route
 * every 30 minutes
 */
void prepare_and_call_API(int &nr_locations, vector<string> &addresses,
						  vector<vector<vector<int>>> &route_matrix);

/**
 * redirect_to_web - Redirects the user to the google maps website
 *
 * @param route: The route (indexes of addresses in order)
 * @param addresses: The vector that stores the addresses
 */
void redirect_to_web(vector<int> &route, vector<string> &addresses);
