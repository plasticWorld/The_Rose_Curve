#include <iostream>
#include <vector>

#include <map>

using namespace std;

void printCompare(const map <string, vector<string>>& routes, const string& stop){
    bool isExist = false;
    for(const auto& route : routes){
            vector<string> list = route.second;
            for(const auto& t : list){
                if (t == stop){
                    cout << t << " ";
                    isExist = true;
                }
            }
            cout << endl;
        }
    if (!isExist) cout << "no interchange\n";
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
            if(true){
                cout << "No stop\n";
            }else {

            }

        }else if (command == "STOPS_FOR_BUS"){
            string bus;
            cin >> bus;
            if (routes.count(bus)==0){
                cout <<"No bus\n";
            }else {
                for(const auto& stop : routes[bus]){
                    cout << "Stop " << stop << ": ";
                       printCompare(routes, stop);
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
