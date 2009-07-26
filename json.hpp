/*
	This file is part of Libmatthewfl.

	Libmatthewfl is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, version 3.

	Libmatthewfl is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Libmatthewfl.  If not, see <http://www.gnu.org/licenses/>.

	Author: Matthew Francis-Landau <matthew@matthewfl.com>
	http://github.com/matthewfl/libmatthewfl/tree/master
	http://matthewfl.github.com/libmatthewfl
	http://matthewfl.com
*/


#ifndef _Matthewfl_json_hpp
#define _Matthewfl_json_hpp

#include <string>
#include <map>
#include <deque>
#include <boost/any.hpp>

#include <boost/algorithm/string/replace.hpp> // used for cleaning up the string

#include <stdlib.h> // used for string to long


// TODO: move this out
namespace matthewfl {
  inline bool compare (const char a, const char b) {
    //return std::string (&a) == std::string (&b);
    return a == b;
  }
  inline bool compare(const char * a, const char * b) {
    return compare(*a, *b);
  }
  inline bool compare(char &a, const char b[]) {
    char c = a;
    return compare(c, *b);
  }
} // namespace matthewfl



namespace matthewfl {

  using boost::any;
  using boost::any_cast;

  class json : public any { // is this bad?
  private:
    enum prase_types {
      TunKnow,
      Tobject,
      Tarray,
      Tstring,
      Tnumber,
      Tbool,
      Tnull
    };
  public:
    class error {
    private:
      std::string d;
    public:
      error(std::string i): d(i) {}
      const std::string what() const { return d; } 
    };
    typedef std::string String;
#ifdef Matthewfl_json_Number_int
    typedef int Number;
#else
    typedef long Number;
#endif
    typedef bool Bool;
    typedef std::map< String , json > Object;
    typedef std::pair< String, json > ObjectPair;
    typedef std::deque< json > Array;

    template <typename T> inline  T * Cast () {
      return any_cast<T>(this);
    }
    template <typename T> inline T & cast () {
      return *Cast<T>();
    }
  
    json() {};
    json(const json & a) { *this = a; }

    template <typename T> json & operator = (const T &o) {
      any a(o);
      a.swap(*this);
      return *this;
    }
    // Array functions
    json & operator [] (const Number &n) { // we assume that it is an array
      return Cast< Array >()->at(n);
    }
    
    void push (json d) {
      any_cast< Array >(this)->push_back(d);
    }
    json pop () {
      Array * a = any_cast< Array >(this);
      json tmp = a->back();
      a->pop_back();
      return tmp;
    }
    json shift() {
      Array * a = any_cast< Array >(this);
      json tmp = a->front();
      a->pop_front();
      return tmp;
    }
    void unshift (json d) {
      any_cast< Array >(this)->push_front(d);
    }
    
    
    // Object functions
    json & operator [] (const String &s) { // we assume that it is an object
      return (*any_cast<Object>(this))[s];
    }
    
    void set(String s, json d) {
      any_cast< Object >(this)->insert(ObjectPair(s, d));
    }
    

    // both
    size_t size () {
      if(type() == typeid(Array))
	return any_cast< Array >(this)->size();
      if(type() == typeid(Object))
	return any_cast< Object >(this)->size();
      if(type() == typeid(String))
	return any_cast< String >(this)->size();
      
      return 0;
    }
    
    // Prase function
    void prase(String &s) {
      unsigned int n = 0;
      *this = prase(s, n);
    }

    String toString () { // return a string of the json object
      if(type() == typeid(Object)) {
      
      }
      if(type() == typeid(Array)) {
      
      }
      if(type() == typeid(String)) {
      
      }
      if(type() == typeid(Number)) {
      
      }
      if(type() == typeid(Bool)) {
	return cast<Bool>() ? "true" : "false"; 
      }
      if(empty()) {
	return "null";
      }
    }
  private:
    // Prase privates
    json prase(String &s, unsigned int &place) {
      json j;
      static const std::string numberList = "1234567890";
      prase_types type = TunKnow;
      while(type == TunKnow) {
	switch(s[place]) {
	case '{':
	  type = Tobject;
	  break;
	case '[':
	  type = Tarray;
	  break;
	case ']':
	case '}':
	  return j; // make a null object
	case '"':
	  type = Tstring;
	  break;
	case 'n':
	  if(compare(s[place+1], 'u')) { // null, could it be anything else in valid json
	    type = Tnull;
	    break;
	  }
	case 't':
	case 'f': // bool
	  type = Tbool;
	  break;
	default:
	  if(numberList.find(s[place]) != std::string::npos) {
	    type = Tnumber;
	  }else
	    place++;
	}
      }
      switch(type) {
      case Tobject: // should be on an {
	j = Object();
	while(!compare(s[place], "}") && place <= s.size()) {
	  for(;!compare(s[place], "\"") && !compare(s[place], "}") && place <= s.size(); place++); // find the start of the name
	  if(compare(s[place], "}") || place >= s.size()) // empty object
	    break;
	  String name = getString(s, place);
	  //for(;!compare(s[place], ":") && !compare(s[place-1], ":"); place++);place++; // move up to the object
	  j.set(name, prase(s, place));
	  for(;!compare(s[place], ",") && !compare(s[place], "}") && !compare(s[place], "\"") && place <= s.size();place++);
	}
	place++;
	break;
      case Tarray: // should be on an [
	j = Array();
	while(!compare(s[place], "]") && place <= s.size()) {
	  // TODO: add empty array, will make a one element array at this point
	  j.push(prase(s, ++place));
	  for(;!compare(s[place], ",") && !compare(s[place], "]") && place <= s.size();place++);
	}
	place++;
	break;
      case Tstring: // should be on an "
	j = getString(s, place);
	break;
      case Tnumber: // should be on an [0-9]
	// add the code for the number
	j = getNumber(s, place);
	break;
      case Tnull: // should be on an n (null)
	place+=4;
	break;
      case Tbool:
	if(compare(s[place], "t")) {
	  j = true;
	  place+=4;
	}else{
	  j = false;
	  place+=5;
	}
      case TunKnow:
	break;
      }
      return j;
    }
    String getString(String &s, unsigned int &place) {
      // string start and end with "
      place++;
      String str;
      while(place <= s.size()) {
	if(compare(s[place], "\"")) {
	  if(!compare(s[place-1], "\\"))
	    break;
	}
	str += s[place++];
      }
      
      // clean up the string here
      boost::replace_all(str, "\\\"", "\"");
      boost::replace_all(str, "\\\'", "\'");
      boost::replace_all(str, "\\n",  "\n");
      boost::replace_all(str, "\\r",  "\r");
      boost::replace_all(str, "\\/", "/");
      boost::replace_all(str, "\\\\", "\\");
      // is there anything else that I need
      place++;
      return str;
      // should return one place after closeing "
    }
    Number getNumber(String &s, unsigned int &place) {
      String str;
      static const std::string numberList = "1234567890";
      while(place <= s.size()) {
	if(numberList.find(s[place]) == std::string::npos) 
	  break;
	str += s[place++];
      }
      return atol(str.c_str());
    }
  };
} // namespace matthewfl

#endif // _Matthewfl_json_hpp
