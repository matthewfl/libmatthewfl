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


#ifndef _PointPool_hpp
#define _PointPool_hpp

///////////////////////////
// made by Matthew FL    //
// http://matthewfl.com  //
// matthew@matthewfl.com //
///////////////////////////

/**
sample:
matthewfl::PointPool point;

something * s;
s = point(new somthing()); // add the pointer to the list
... more code ...
point.empty(); // delete all of the pointer on the list

 */

namespace matthewfl {

class PointPool {
public:
  PointPool (): content(new placeHolder()) {}
  ~PointPool () {
    delete content;
  }
  template<typename T> T * add(T * d) {
    /// add a pointer to the list to be removed later
    content = new holder<T>(d, content);
    return d;
  }
  template <typename T> inline T * operator() (T * d)  {
    /// add a pointer to the list to be removed later
    return add<T>(d);
  }
  void clear () {
    /// remove everything from the list
    delete content;
    content = new placeHolder;
  }
  void inline clean () {
    /// remove everything from the list
    clear();
  }
  void inline empty () {
    /// remove everything from the list
    clear();
  }
  bool isEmpty () {
    /// returns true is nothing is in the list
    return content->empty();
  }
private:
  
  class placeHolder {
  public:
    virtual ~placeHolder() {}
    virtual bool empty () const {return true;}
    //virtual unsigned int size () const {return 0;}
  };

  template <typename T> class holder: public placeHolder {
  public:
    holder(T * d, placeHolder * last): data(d), next(last) {}
    ~holder() { delete next; delete data; }
    bool empty () const {return false;}
    //unsigned int size() const {return next->size() + 1;}
  private:
    T * data;
    placeHolder * next;
  };

  placeHolder * content;
};

} // namespace matthewfl

#endif // _PointPool_hpp
