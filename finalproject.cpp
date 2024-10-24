#include <iostream>
#include <string>
#include <map>
#include <climits>
#include <vector>
#include <algorithm> // For sort and transform

using namespace std;

const int V = 10; // Number of locations

// Function for login
bool login() {
    string username, password, phoneNumber;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Check if username and password match the allowed credentials
    if ((username == "Ashutosh" && password == "Ashutosh") ||
        (username == "Satkar" && password == "Satkar")) {

        // Ask for phone number and validate it
        cout << "Enter your 10-digit phone number: ";
        cin >> phoneNumber;

        // Validate phone number length and digits
        if (phoneNumber.length() != 10) {
            cout << "Invalid phone number. Please enter a 10-digit number.\n";
            return false;
        }

        // Check if all characters in phoneNumber are digits
        for (char c : phoneNumber) {
            if (!isdigit(c)) {
                cout << "Invalid phone number. Please enter a 10-digit number.\n";
                return false;
            }
        }

        cout << "Login successful!\n";
        return true;
    } else {
        cout << "Invalid credentials. Access denied!\n";
        return false;
    }
}

// Function to display choices to the user
int getUserChoice() {
    int choice;
    cout << "\nWhat would you like to do?\n";
    cout << "1. Emergency: I need an ambulance\n";
    cout << "2. Check the nearest hospital and its distance\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;
    return choice;
}

// Find the index of the user's location
int getLocationIndex(const string& location, const vector<string>& locations) {
    for (int i = 0; i < V; i++) {
        if (locations[i] == location) {
            return i;
        }
    }
    return -1; // Invalid location
}

// Dijkstra's algorithm for shortest path
void dijkstra(int graph[V][V], int src, const vector<string>& hospitals) {
    vector<int> dist(V, INT_MAX);
    vector<bool> visited(V, false);

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = -1;

        // Find the minimum distance node
        for (int i = 0; i < V; i++) {
            if (!visited[i] && (u == -1 || dist[i] < dist[u])) {
                u = i;
            }
        }

        visited[u] = true;

        // Update the distance of neighboring nodes
        for (int v = 0; v < V; v++) {
            if (graph[u][v] && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Find the two nearest hospitals
    cout << "\nThe two nearest hospitals with ambulance service:\n";
    vector<pair<int, int>> nearestHospitals;
    for (int i = 0; i < V; i++) {
        if (i != src) {
            nearestHospitals.push_back({dist[i], i});
        }
    }
    
    // Sort by distance and show the two nearest hospitals
    sort(nearestHospitals.begin(), nearestHospitals.end());
    for (int i = 0; i < 2 && i < nearestHospitals.size(); i++) {
        cout << i + 1 << ". " << hospitals[nearestHospitals[i].second] << " - " << nearestHospitals[i].first << " km\n";
    }

    // Ask user to select a hospital
    int hospitalChoice;
    cout << "\nEnter the number of the hospital you want an ambulance from: ";
    cin >> hospitalChoice;

    if (hospitalChoice == 1 || hospitalChoice == 2) {
        cout << "Processing... We will reach your location soon. Please be ready!\n";
    } else {
        cout << "Invalid selection. Exiting...\n";
    }
}

int main() {
    if (!login()) {
        return 0; // Terminate if login fails
    }

    // List of locations
    vector<string> locations = {
        "Gongabu", "Durbarmag", "Buddhanilkantha", "Chabahil", "Teku",
        "Swayambhunath", "Kalanki", "Kavresthali", "Jawalakhel", "Pulchowk"
    };

    // Hospital names near each location
    vector<string> hospitals = {
        "Green City Hospital", "Norvic Hospital", "Grande International Hospital",
        "Om Hospital", "Bir Hospital", "Manmohan Memorial Hospital", 
        "Nepal Police Hospital", "Kirtipur Hospital", "Lalitpur Hospital", "Alka Hospital"
    };

    // Hypothetical distance graph between 10 places (in km)
    int graph[V][V] = {
        {0, 4, 7, 3, 5, 6, 8, 12, 10, 9},  // Gongabu
        {4, 0, 6, 5, 2, 7, 9, 11, 8, 7},   // Durbarmag
        {7, 6, 0, 3, 7, 8, 12, 3, 9, 10},  // Buddhanilkantha
        {3, 5, 3, 0, 4, 5, 9, 7, 8, 6},    // Chabahil
        {5, 2, 7, 4, 0, 5, 10, 9, 7, 8},   // Teku
        {6, 7, 8, 5, 5, 0, 4, 13, 10, 9},  // Swayambhunath
        {8, 9, 12, 9, 10, 4, 0, 15, 12, 11}, // Kalanki
        {12, 11, 3, 7, 9, 13, 15, 0, 8, 9}, // Kavresthali
        {10, 8, 9, 8, 7, 10, 12, 8, 0, 2},  // Jawalakhel
        {9, 7, 10, 6, 8, 9, 11, 9, 2, 0}    // Pulchowk
    };

    // Ask user for choice
    int choice = getUserChoice();
    if (choice != 1 && choice != 2) {
        cout << "Invalid choice. Exiting...\n";
        return 0;
    }

    // Ask user for their location
    cout << "\nAvailable locations:\n";
    for (int i = 0; i < V; i++) {
        cout << locations[i] << endl;
    }

    string location;
    cout << "\nEnter your location from the list: ";
    cin.ignore(); // To ignore any leftover newline from previous input
    getline(cin, location); // Use getline to capture the whole line

    // Transform location to ensure case-insensitive comparison
    transform(location.begin(), location.end(), location.begin(), ::tolower);

    // Check for valid location
    bool validLocation = false;
    for (int i = 0; i < V; i++) {
        if (locations[i].compare(0, location.size(), location) == 0) {
            validLocation = true;
            location = locations[i]; // Match the exact location
            break;
        }
    }

    if (!validLocation) {
        cout << "Invalid location entered. Exiting...\n";
        return 0;
    }

    // If the user needs an ambulance or wants the nearest hospital
    int locationIndex = getLocationIndex(location, locations);
    dijkstra(graph, locationIndex, hospitals); // Show nearest hospitals

    return 0;
}