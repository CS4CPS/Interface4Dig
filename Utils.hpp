#include "api/Api.hpp"

#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <vector>
#include <string>
#include <iostream>

// Api
using api::DataObject;
using api::Application;
using api::Api;
using api::Value;

// provides an API instance
class ApiFixture
{
public:
  ApiFixture();
  ~ApiFixture();

  static api::Api* GetInstance() {return instance;}
private:
  ApiFixture(const ApiFixture&) {};
  static api::Api* instance;
};


//resource guard that ensures that value pointers are released
class ValueGuard
{
public:
  ValueGuard(const api::Value* val) : m_val(val){};
  ~ValueGuard();

  const api::Value* operator->() const  { return m_val; }

private:
  ValueGuard() {};
  ValueGuard(const ValueGuard&) {};
  const api::Value* m_val;
};


//some utility functions
class Utils
{
public:
  static std::vector<api::DataObject*> GetChildren(api::DataObject* parent, std::string classNameFilter);
};
#endif
