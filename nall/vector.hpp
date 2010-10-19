#ifndef NALL_VECTOR_HPP
#define NALL_VECTOR_HPP

#include <nall/C++98.hpp>
#include <new>
#include <nall/algorithm.hpp>
#include <nall/bit.hpp>
#include <nall/concept.hpp>
#include <nall/foreach.hpp>
#include <nall/utility.hpp>

namespace nall {
  //linear_vector
  //memory: O(capacity * 2)
  //
  //linear_vector uses placement new + manual destructor calls to create a
  //contiguous block of memory for all objects. accessing individual elements
  //is fast, though resizing the array incurs significant overhead.
  //reserve() overhead is reduced from quadratic time to amortized constant time
  //by resizing twice as much as requested.
  //
  //if objects hold memory address references to themselves (introspection), a
  //valid copy constructor will be needed to keep pointers valid.

  template<typename T> class linear_vector {
  protected:
    T *pool;
    uint64_t poolsize, objectsize;

  public:
    uint64_t size() const { return objectsize; }
    uint64_t capacity() const { return poolsize; }

    void reset() {
      if(pool) {
        for(uint64_t i = 0; i < objectsize; i++) pool[i].~T();
        free(pool);
      }
      pool = 0;
      poolsize = 0;
      objectsize = 0;
    }

    void reserve(uint64_t newsize) {
      newsize = bit::round(newsize);  //round to nearest power of two (for amortized growth)

      T *poolcopy = (T*)malloc(newsize * sizeof(T));
      for(uint64_t i = 0; i < min(objectsize, newsize); i++) new(poolcopy + i) T(pool[i]);
      for(uint64_t i = 0; i < objectsize; i++) pool[i].~T();
      free(pool);
      pool = poolcopy;
      poolsize = newsize;
      objectsize = min(objectsize, newsize);
    }

    void resize(uint64_t newsize) {
      if(newsize > poolsize) reserve(newsize);

      if(newsize < objectsize) {
        //vector is shrinking; destroy excess objects
        for(uint64_t i = newsize; i < objectsize; i++) pool[i].~T();
      } else if(newsize > objectsize) {
        //vector is expanding; allocate new objects
        for(uint64_t i = objectsize; i < newsize; i++) new(pool + i) T;
      }

      objectsize = newsize;
    }

    void append(const T data) {
      if(objectsize + 1 > poolsize) reserve(objectsize + 1);
      new(pool + objectsize++) T(data);
    }

    template<typename U> void insert(uint64_t index, const U list) {
      linear_vector<T> merged;
      for(uint64_t i = 0; i < index; i++) merged.append(pool[i]);
      foreach(item, list) merged.append(item);
      for(uint64_t i = index; i < objectsize; i++) merged.append(pool[i]);
      operator=(merged);
    }

    void insert(uint64_t index, const T item) {
      insert(index, linear_vector<T>(item));
    }

    void remove(uint64_t index, uint64_t count = 1) {
      for(uint64_t i = index; count + i < objectsize; i++) {
        pool[i] = pool[count + i];
      }
      if(count + index >= objectsize) resize(index);  //every element >= index was removed
      else resize(objectsize - count);
    }

    inline T& operator[](uint64_t index) {
      if(index >= objectsize) resize(index + 1);
      return pool[index];
    }

    inline const T& operator[](uint64_t index) const {
      //if(index >= objectsize) throw "vector[] out of bounds";
      return pool[index];
    }

    //copy
    inline linear_vector<T>& operator=(const linear_vector<T> &source) {
      reset();
      reserve(source.capacity());
      resize(source.size());
      for(uint64_t i = 0; i < source.size(); i++) operator[](i) = source.operator[](i);
      return *this;
    }

    linear_vector(const linear_vector<T> &source) : pool(0), poolsize(0), objectsize(0) {
      operator=(source);
    }

    //construction
    linear_vector() : pool(0), poolsize(0), objectsize(0) {
    }

    linear_vector(std::initializer_list<T> list) : pool(0), poolsize(0), objectsize(0) {
      for(const T *p = list.begin(); p != list.end(); ++p) append(*p);
    }

    ~linear_vector() {
      reset();
    }
  };

  //pointer_vector
  //memory: O(1)
  //
  //pointer_vector keeps an array of pointers to each vector object. this adds
  //significant overhead to individual accesses, but allows for optimal memory
  //utilization.
  //
  //by guaranteeing that the base memory address of each objects never changes,
  //this avoids the need for an object to have a valid copy constructor.

  template<typename T> class pointer_vector {
  protected:
    T **pool;
    uint64_t poolsize, objectsize;

  public:
    uint64_t size() const { return objectsize; }
    uint64_t capacity() const { return poolsize; }

    void reset() {
      if(pool) {
        for(uint64_t i = 0; i < objectsize; i++) { if(pool[i]) delete pool[i]; }
        free(pool);
      }
      pool = 0;
      poolsize = 0;
      objectsize = 0;
    }

    void reserve(uint64_t newsize) {
      newsize = bit::round(newsize);  //round to nearest power of two (for amortized growth)

      for(uint64_t i = newsize; i < objectsize; i++) {
        if(pool[i]) { delete pool[i]; pool[i] = 0; }
      }

      pool = (T**)realloc(pool, newsize * sizeof(T*));
      for(uint64_t i = poolsize; i < newsize; i++) pool[i] = 0;
      poolsize = newsize;
      objectsize = min(objectsize, newsize);
    }

    void resize(uint64_t newsize) {
      if(newsize > poolsize) reserve(newsize);

      for(uint64_t i = newsize; i < objectsize; i++) {
        if(pool[i]) { delete pool[i]; pool[i] = 0; }
      }

      objectsize = newsize;
    }

    void append(const T data) {
      if(objectsize + 1 > poolsize) reserve(objectsize + 1);
      pool[objectsize++] = new T(data);
    }

    template<typename U> void insert(uint64_t index, const U list) {
      pointer_vector<T> merged;
      for(uint64_t i = 0; i < index; i++) merged.append(*pool[i]);
      foreach(item, list) merged.append(item);
      for(uint64_t i = index; i < objectsize; i++) merged.append(*pool[i]);
      operator=(merged);
    }

    void insert(uint64_t index, const T item) {
      insert(index, pointer_vector<T>(item));
    }

    void remove(uint64_t index, uint64_t count = 1) {
      for(uint64_t i = index; count + i < objectsize; i++) {
        *pool[i] = *pool[count + i];
      }
      if(count + index >= objectsize) resize(index);  //every element >= index was removed
      else resize(objectsize - count);
    }

    inline T& operator[](uint64_t index) {
      if(index >= objectsize) resize(index + 1);
      if(!pool[index]) pool[index] = new T;
      return *pool[index];
    }

    inline const T& operator[](uint64_t index) const {
      //if(index >= objectsize || !pool[index]) throw "vector[] out of bounds";
      return *pool[index];
    }

    //copy
    inline pointer_vector<T>& operator=(const pointer_vector<T> &source) {
      reset();
      reserve(source.capacity());
      resize(source.size());
      for(uint64_t i = 0; i < source.size(); i++) operator[](i) = source.operator[](i);
      return *this;
    }

    pointer_vector(const pointer_vector<T> &source) : pool(0), poolsize(0), objectsize(0) {
      operator=(source);
    }

    //construction
    pointer_vector() : pool(0), poolsize(0), objectsize(0) {
    }

    pointer_vector(std::initializer_list<T> list) : pool(0), poolsize(0), objectsize(0) {
      for(const T *p = list.begin(); p != list.end(); ++p) append(*p);
    }

    ~pointer_vector() {
      reset();
    }
  };

  template<typename T> struct has_size< linear_vector<T> > { enum { value = true }; };
  template<typename T> struct has_size< pointer_vector<T> > { enum { value = true }; };
}

#endif
