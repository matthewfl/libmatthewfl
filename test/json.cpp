#! /usr/local/bin/cxx
#include "../trunk/json.hpp"
#include <iostream>
using namespace std;

using namespace matthewfl;
int main (int argc, char * argv[]) {
  matthewfl::json::String s;
  s = "{\"hi\":{\"a\":[\"dd\", \"a\", 12345, null], \"b\":[true, false]}}";
  cout << "start\n";
  matthewfl::json j;
  j.prase(s);
  cout << "waback\n";
  cout << any_cast< json::Object >(j).size() << endl;
  //  cout << j["hi"]["a"][1].cast< json::String >() <<endl;
  cout << boost::any_cast<json::Number>(j["hi"]["a"][2]) << endl;
  cout << j["hi"]["a"][3].empty() <<"\naaa\n";
  cout << j["hi"].size() <<endl;
  cout << j["hi"]["b"][0].cast<bool>() << endl;
  //cout << typeid(json::Number).name() <<endl;
  //  cout << j.type() == typeid(matthewfl::json::Object);
  //json d;
  //json::Object o = any_cast<json::Object>(d);
  //cout << o.size();
  //  cout << any_cast <json::String> (any_cast <json::Object>(j)["hi"]);
  //  cout << j.type().name() << endl
  cout << "return\n";
  return 0;
}