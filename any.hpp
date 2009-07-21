#ifndef _Matthewfl_any_hpp
#define _Matthewfl_any_hpp

#include <typeinfo>

namespace matthewfl {
  class any {
  private:
    class placeHolder {
    public:
      virtual const std::type_info & type () const = 0;
      virtual const placeHolder * clone const = 0;
    };
    template <typename T> class Holder : public placeHolder {
    public:
      T data;
      virtual const std::type_info & type () const {
	return typeid(T);
      }
      virtual const placeHolder * clone const {
	return new Holder<T>(data);
      }
      Holder(T d): data(d) {}
    };
    placeHolder * _;
  public:
    ~any() { if(_ != NULL) delete _;
    any ():_(NULL) {}
    template <typename T> any(T &d) {
      _ = new Holder<T>(d);
    }
    any(any &a) {
      if(a._ == NULL)
	_ = NULL;
      else
	_ = a._->clone();
    }

    any swap (any &a) {
      placeHolder * tmp = a._;
      a._ = _;
      _ = tmp;
    }
    const std::type_info type () const {
      if(_ == NULL)
	return typeid(void);
      else
	_->type();
    }
    any & operator = (any &d) {
      if(d._ != NULL)
	_ = d._->clone();
    }
    template <typename T> any & operator = (T &d) {
      any a(d);
      swap(a);
      return *this;
    }
    bool empty () const {
      return !_;
    }
    template <typename T> T & cast () {
      return _ && typeid(T) == type() : _.data ? 0;
    }
  };
} // namespace matthewfl

#endif // _Matthewfl_any_hpp
