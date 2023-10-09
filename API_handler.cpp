// Copyright 2023 Baldovin-Razvan-Mihai-Marian @2amazing4me

#ifndef API_HANDLER_H
#define API_HANDLER_H

#include "API_handler.h"
#include "routes.h"

const char *API_KEY = std::getenv("MAPS_API_KEY");

void get_addresses(int &nr_locations, vector<string> &addresses)
{
	cout << "Enter number of locations: ";
	cin >> nr_locations;
	cin.ignore();

	for (int i = 0; i < nr_locations; ++i) {
		cout << "Enter address " << i + 1 << ": ";
		string address;
		getline(cin, address);

		addresses.push_back(address);
	}
}

static string timeToISO8601(int iteration)
{
	using namespace std::chrono;

	// Get the current time as a time_point
	system_clock::time_point now = system_clock::now();

	// Extract the seconds since epoch as a time_t object
	time_t seconds_since_epoch = system_clock::to_time_t(now);
	seconds_since_epoch += iteration * 1800 + 10;

	// Extract the nanoseconds from the current time
	auto since_epoch = now.time_since_epoch();
	auto s = duration_cast<seconds>(since_epoch);
	auto ns = duration_cast<nanoseconds>(since_epoch - s);

	// Use gmtime to get the time components in UTC
	std::tm timeinfo;
	gmtime_r(&seconds_since_epoch, &timeinfo); // Thread-safe version of gmtime

	ostringstream oss;

	// Output the date and time in ISO 8601 format
	oss << put_time(&timeinfo, "%Y-%m-%dT%H:%M:%S");

	// Append the fractional seconds (nanoseconds)
	oss << '.' << setw(9) << setfill('0') << ns.count();

	// Append the 'Z' to indicate UTC time
	oss << 'Z';

	return oss.str();
}

string getJSONPayload(vector<string> &addresses, int nr_locations, int add_time)
{
	// Start the origins section
	string payload = "'{\n  \"origins\": [\n";

	// TODO: Add current location (probably by latitute and longitude)

	// Adds all the origin points (current location + all delivery addresses)
	for (int i = 0; i < nr_locations; ++i) {
		payload += "    {\n";
		payload += "      \"waypoint\": {\n";
		payload += "        \"address\": \"" + addresses[i] + "\"\n";
		payload += "      }\n";
		payload += "    }";

		// Only add the comma if there is another origin point
		if (i + 1 < nr_locations)
			payload += ",\n";
	}

	// Start the destinations section
	payload += "\n";
	payload += "  ],\n";
	payload += "    \"destinations\": [\n";

	// Adds all the destination points (all delivery addresses, no need for the
	// current address)
	for (int i = 0; i < nr_locations; ++i) {
		payload += "    {\n";
		payload += "      \"waypoint\": {\n";
		payload += "        \"address\": \"" + addresses[i] + "\"\n";
		payload += "      }\n";
		payload += "    }";

		// Only add the comma if there is another destination point
		if (i + 1 < nr_locations)
			payload += ",\n";
	}

	// End the locations section
	payload += "\n";
	payload += "  ],\n";
	// Set the travel mode to driving and calculate route time based on traffic
	payload += "  \"travelMode\": \"DRIVE\",\n";
	payload += "  \"routingPreference\": \"TRAFFIC_AWARE_OPTIMAL\",\n";
	payload += "  \"departureTime\": \"" + timeToISO8601(add_time) + "\"\n";
	payload += "}' \\\n";

	return payload;
}

void prepare_and_call_API(int &nr_locations, vector<string> &addresses,
						  vector<vector<vector<int>>> &route_matrix)
{
	get_addresses(nr_locations, addresses);

	route_matrix.resize(
		4, vector<vector<int>>(nr_locations, vector<int>(nr_locations, 0)));

	for (int i = 0; i < 4; ++i) {
		// Generates the curl command to get the Route Matrix using Google Maps'
		// API
		string command =
			"curl -X POST -d " + getJSONPayload(addresses, nr_locations, i) +
			"-H 'Content-Type: application/json' " +
			"-H 'X-Goog-Api-Key: " + API_KEY + "' \\\n" +
			"-H 'X-Goog-FieldMask: originIndex,destinationIndex,duration,"
			"distanceMeters,status,condition' \\\n" +
			"'https://routes.googleapis.com/distanceMatrix/"
			"v2:computeRouteMatrix' "
			"| jq '.[] | [.originIndex, .destinationIndex, .duration]' | tr "
			"-d \"[],nulls\\\"\" > answer.json";

		system(command.c_str());

		// cout << command << endl;

		get_route_durations(route_matrix, i);
	}
}

void redirect_to_web(vector<int> &route, vector<string> &addresses)
{
	// Generate the command template
	string command = "xdg-open \"https://www.google.com/maps/dir/";

	// Add the addresses to the command
	for (int i = 0; i < route.size(); ++i) {
		// Replaces spaces with '+' in addresses
		int pos = addresses[route[i]].find(" ");
		while (pos != string::npos) {
			addresses[route[i]][pos] = '+';
			pos = addresses[route[i]].find(" ");
		}

		command += addresses[route[i]];

		if (i + 1 < route.size())
			command += "/";
	}

	command += "\"";
	// cout << command << endl;
	system(command.c_str());
}

#endif // API_HANDLER_H