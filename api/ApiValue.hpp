///
/// @file ApiValue.hpp  Value class for PowerFactory API
///
/// @responsible Christoph Wolter
///
#ifndef __APIVALUE_HPP_INCLUDE
#define __APIVALUE_HPP_INCLUDE

namespace api {

  class DataObject;
  class ValueImpl;

  class Value
  {
  public:

    enum Type {
      UNKNOWN, INTEGER, DOUBLE, STRING, INTEGER64, DATAOBJECT, VECTOR, DOUBLEMATRIX, VALUE /* only used for values stored inside values*/
    };

    const Type GetType() const;

    Value(const Type type);
    Value(const int val);
    Value(const __int64 val);
    Value(const double val);
    Value(const char* val);
    Value(DataObject* val);
    Value(const unsigned int countCols, const unsigned int countRows); //constructor for type matrix

    //data access
    int GetInteger(int* error=0) const;
    __int64 GetInteger64(int* error=0) const;
    double GetDouble(int* error=0) const;
    const char* GetString(int* error=0) const;
    DataObject* GetDataObject(int* error=0) const;

    //value vector
    void VecInsertInteger(const int val, int* error=0);
    void VecInsertInt64(const __int64 val, int* error=0);
    void VecInsertDouble(const double val, int* error=0);
    void VecInsertString(const char* val, int* error=0);
    void VecInsertDataObject(DataObject* val, int* error=0);
    void VecInsertValue(const Value* val, int* error=0);

    unsigned int VecGetSize(int* error=0) const;
    Type VecGetType(unsigned int idx, int* error=0) const; //unclear if this function will be made available

    int VecGetInteger(const unsigned int idx, int* error=0) const;
    __int64 VecGetInteger64(const unsigned int idx, int* error=0) const;
    double VecGetDouble(const unsigned int idx, int* error=0) const;
    const char* VecGetString(const unsigned int idx, int* error=0) const;
    const Value* VecGetValue(const unsigned int idx, int* error=0) const;
    DataObject* VecGetDataObject(const unsigned int idx, int* error=0) const;

    unsigned int MatGetRowCount(int* error=0) const;
    unsigned int MatGetColCount(int* error=0) const;
    void MatSetDouble(const unsigned int row, const unsigned int col, const double val, int* error=0);
    double MatGetDouble(const unsigned int row, const unsigned int col, int* error=0) const;

    ~Value();

  private:
    Value(const Value&) : m_data(0){}; //not allowed
    const Value& operator=(const Value&) {return *this;} //not allowed

    ValueImpl* m_data;
  };
}
#endif // __APIVALUE_HPP_INCLUDE