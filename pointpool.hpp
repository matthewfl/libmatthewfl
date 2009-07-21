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
    /// add a pointer to the list to be removed latter
    content = new holder<T>(d, content);
    return d;
  }
  template <typename T> inline T * operator() (T * d)  {
    /// add a pointer to the list to be removed latter
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
