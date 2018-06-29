#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

void printCompare(const map <string, vector<string>>& routes, const string& stop, const string& bus){
    bool isExist = false;
    for(const auto& route : routes){
            vector<string> list = route.second;
            for(const auto& stop_name : list){
                if (stop_name == stop && bus != route.first){
                    cout << route.first << " ";
                    isExist = true;
                }
            }
        }
    if (!isExist) cout << "no interchange";
}

void isBuses(const map <string, vector<string>>& routes, const string& stop){
    bool bus = false;
    for(const auto& route : routes){
        if(find(route.second.begin(), route.second.end(), stop) != route.second.end()){
           cout << route.first << " ";
           bus = true;
        }
    }
    if(!bus) cout << "No stop";
}

int main() {
    int n;
    cin >> n;
    map <string, vector<string>> routes;

    for (int i = 0; i < n; i++){
        string command;
        cin >> command;

        if(command == "NEW_BUS"){
            string tmp, tmp_s;
            vector <string> temp;
            int c(0);
            cin >> tmp;
            cin >> c;
            for (int i = 0; i < c; i++){
                cin >> tmp_s;
                temp.push_back(tmp_s);
            }
            routes[tmp] = temp;

        }else if(command == "BUSES_FOR_STOP"){
            string stop;
            cin >> stop;
            isBuses(routes, stop);
            cout << endl;
        }else if (command == "STOPS_FOR_BUS"){
            string bus;
            cin >> bus;
            if (routes.count(bus)==0){
                cout <<"No bus\n";
            }else {
                for(const auto& stop : routes[bus]){
                    cout << "Stop " << stop << ": ";
                       printCompare(routes, stop, bus);
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
