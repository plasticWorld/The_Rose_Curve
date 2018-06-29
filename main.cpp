#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
using namespace std;

void STOPS_FOR_BUS(const map <int, vector<string>>& routesInputOrder,
                  const string& stop, const string& bus, const vector<string>& order){
    bool isExist = false;
    for(const auto& route : routesInputOrder){
            for(const auto& stop_name : route.second){
                if (stop_name == stop && bus != order[route.first]){
                    cout << order[route.first] << " ";
                    isExist = true;
                }
            }
        }
    if (!isExist) cout << "no interchange";
}

void BUSES_FOR_STOP(const map <int, vector<string>>& routesInputOrder,
             const string& stop, const vector<string>& order, bool& areThere){
    for (const auto& route : routesInputOrder){
        if(find(route.second.begin(), route.second.end(), stop) != route.second.end()){
            cout << order[route.first] << " ";
            areThere = false;
        }
    }
}


int main() {
    int n, j(0);
    string command, stop, bus;
    cin >> n;

    map <string, vector<string>> routes;
    map <int, vector<string>> routesInputOrder;
    vector<string> order;

    for (int i = 0; i < n; i++){
        cin >> command;

        if(command == "NEW_BUS"){
            string busNumber, busStop;
            cin >> busNumber;
            order.push_back(busNumber);

            int c(0);
            cin >> c;

            for (int i = 0; i < c; i++){
                cin >> busStop;
                routes[busNumber].push_back(busStop);
                routesInputOrder[j].push_back(busStop);
            }
            j++;

        }else if(command == "BUSES_FOR_STOP"){
            cin >> stop;
            bool areThere = true;
            BUSES_FOR_STOP(routesInputOrder, stop, order, areThere);
            if (areThere) cout << "No stop";
            cout << endl;

        }else if (command == "STOPS_FOR_BUS"){
            cin >> bus;
            if (routes.count(bus)==0){
                cout <<"No bus\n";
            }else {
                for(const auto& stop : routes[bus]){
                    cout << "Stop " << stop << ": ";
                       STOPS_FOR_BUS(routesInputOrder, stop, bus, order);
                    cout << endl;
                }
            }

        }else if (command == "ALL_BUSES"){
            if(routes.empty()){
                cout << "No buses\n";
            }else {
                for (const auto& e : routes){
                    cout << "Bus " << e.first << ": ";
                    for(const auto& s : e.second){
                        cout << s << " ";
                    }
                    cout << endl;
                }
            }
        }
    }

  return 0;
}
