#ifndef NALL_CONFIG_HPP
#define NALL_CONFIG_HPP

#include <nall/file.hpp>
#include <nall/string.hpp>
#include <nall/vector.hpp>

namespace nall {
  namespace configuration_traits {
    template<typename T> struct is_boolean { enum { value = false }; };
    template<> struct is_boolean<bool> { enum { value = true }; };

    template<typename T> struct is_signed { enum { value = false }; };
    template<> struct is_signed<signed> { enum { value = true }; };

    template<typename T> struct is_uint64_t { enum { value = false }; };
    template<> struct is_uint64_t<uint64_t> { enum { value = true }; };

    template<typename T> struct is_double { enum { value = false }; };
    template<> struct is_double<double> { enum { value = true }; };

    template<typename T> struct is_string { enum { value = false }; };
    template<> struct is_string<string> { enum { value = true }; };
  }

  class configuration {
  public:
    enum type_t { boolean_t, signed_t, uint64_t_t, double_t, string_t, unknown_t };
    struct item_t {
      uintptr_t data;
      string name;
      string desc;
      type_t type;

      string get() const {
        switch(type) {
          case boolean_t:  return string() << *(bool*)data;
          case signed_t:   return string() << *(signed*)data;
          case uint64_t_t: return string() << *(uint64_t*)data;
          case double_t:   return string() << *(double*)data;
          case string_t:   return string() << "\"" << *(string*)data << "\"";
        }
        return "???";
      }

      void set(string s) {
        switch(type) {
          case boolean_t:  *(bool*)data = (s == "true");      break;
          case signed_t:   *(signed*)data = strsigned(s);     break;
          case uint64_t_t: *(uint64_t*)data = struint64_t(s); break;
          case double_t:   *(double*)data = strdouble(s);     break;
          case string_t:   s.trim("\""); *(string*)data = s;  break;
        }
      }
    };
    linear_vector<item_t> list;

    template<typename T>
    void attach(T &data, const char *name, const char *desc = "") {
      uint64_t n = list.size();
      list[n].data = (uintptr_t)&data;
      list[n].name = name;
      list[n].desc = desc;

      if(configuration_traits::is_boolean<T>::value) list[n].type = boolean_t;
      else if(configuration_traits::is_signed<T>::value) list[n].type = signed_t;
      else if(configuration_traits::is_uint64_t<T>::value) list[n].type = uint64_t_t;
      else if(configuration_traits::is_double<T>::value) list[n].type = double_t;
      else if(configuration_traits::is_string<T>::value) list[n].type = string_t;
      else list[n].type = unknown_t;
    }

    virtual bool load(const char *filename) {
      string data;
      if(data.readfile(filename) == true) {
        data.replace("\r", "");
        lstring line;
        line.split("\n", data);

        for(uint64_t i = 0; i < line.size(); i++) {
          if(optional<uint64_t> position = qstrpos(line[i], "#")) line[i][position()] = 0;
          if(!qstrpos(line[i], " = ")) continue;

          lstring part;
          part.qsplit(" = ", line[i]);
          part[0].trim();
          part[1].trim();

          for(uint64_t n = 0; n < list.size(); n++) {
            if(part[0] == list[n].name) {
              list[n].set(part[1]);
              break;
            }
          }
        }

        return true;
      } else {
        return false;
      }
    }

    virtual bool save(const char *filename) const {
      file fp;
      if(fp.open(filename, file::mode_write)) {
        for(uint64_t i = 0; i < list.size(); i++) {
          string output;
          output << list[i].name << " = " << list[i].get();
          if(list[i].desc != "") output << " # " << list[i].desc;
          output << "\r\n";
          fp.print(output);
        }

        fp.close();
        return true;
      } else {
        return false;
      }
    }
  };
}

#endif
