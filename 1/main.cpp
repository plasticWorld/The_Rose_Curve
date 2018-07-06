#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

class Person {
public:
  void ChangeFirstName(int year, const string& first_name) {
      list_f[year]=first_name;
  }
  void ChangeLastName(int year, const string& last_name) {
      list_l[year]=last_name;
  }
  string GetFullName(int year) {
      int target_year = year;
      for(auto& y: list_f, list_l){

      }
      if(list_f.empty() && list_l.empty()){
          return "Incognito";
      }else {

      }







      if(list_f.count(year) == 0 && list_l.count(year) != 0){
          return list_l[year] + "with unknown first name";
      }else if(list_l.count(year) == 0 && list_f.count(year) != 0){
          return list_f[year] + "with unknown last name";
      }else {
          return list_f[year] + list_l[year];
      }
  }
private:
  string first_name;
  string last_name;
  map<int, string> list_f;
  map<int, string> list_l;
  // приватные поля
};

int main() {
  Person person;

  person.ChangeFirstName(1965, "Polina");
  person.ChangeLastName(1967, "Sergeeva");
  for (int year : {1900, 1965, 1990}) {
    cout << person.GetFullName(year) << endl;
  }

  person.ChangeFirstName(1970, "Appolinaria");
  for (int year : {1969, 1970}) {
    cout << person.GetFullName(year) << endl;
  }

  person.ChangeLastName(1968, "Volkova");
  for (int year : {1969, 1970}) {
    cout << person.GetFullName(year) << endl;
  }

  return 0;
}

