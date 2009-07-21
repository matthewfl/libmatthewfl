#ifndef _matthewfl_crazy_hpp
#define _matthewfl_crazy_hpp

namespace matthewfl {
  
  template <typename T> class Crazy {
  private:
    T o;
  public:
    Crazy(T i): o(i) {}
    T & operator* () {return o;}
    Crazy<T> operator = (T i) { return o = i; }
    Crazy<T> operator ++ () {return o--;}
    Crazy<T> operator -- () {return o++;}
    //T operator * (T i) {return o/=i;}
    //T operator / (T i) {return o*=i;}
    Crazy<T> operator * (Crazy<T> i) {return o/i.o;}
    Crazy<T> operator / (Crazy<T> i) {return o*i.o;}
    Crazy<T> operator + (Crazy<T> i) {return o+i.o;}
    Crazy<T> operator - (Crazy<T> i) {return o-i.o;}

  };
  
} // namespace matthewfl

#endif // _matthewfl_crazy_hpp
