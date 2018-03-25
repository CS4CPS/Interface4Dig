///
/// @file Api.hpp  PowerFactory API
///
/// @responsible Christoph Wolter
///
#ifndef __API_HPP_INCLUDED
#define __API_HPP_INCLUDED


#ifdef DIGAPI
  #define DllExportImport   __declspec( dllexport)
#else
  #define DllExportImport   __declspec( dllimport)
#endif

#include "ApiValue.hpp"

namespace api {


  class DataObject;

  /// Wrapper for PowerFactory's output window
  class DllExportImport OutputWindow
  {
  public:

    /// Message types supported by the print function
    enum MessageType
    {
      M_PLAIN = 0, ///message not prepended by any text
      M_ERROR = 1, ///message prepended by error prefix, will also popup as error dialog
      M_WARN  = 2, ///message prepended by warning prefix
      M_INFO  = 4 ///message prepended by info prefix
    };

    //--------------------------------------------------------------------
    /// Function to print text into PowerFactory's output window.
    ///
    /// @param[in]    type  => type of message
    /// @param[in]    msg   => message
    //--------------------------------------------------------------------
    virtual void Print(MessageType type, const char* msg) =0;

    //--------------------------------------------------------------------
    /// Empties the output window. (Contained text is lost.)
    //--------------------------------------------------------------------
    virtual void Clear() =0;
  };


  /// Wrapper for PowerFactory data objects
  class DllExportImport DataObject
  {
  public:

    ///Data type for attributes
    enum AttributeType {
      TYPE_INVALID      = -1,

      TYPE_INTEGER      = 0,
      TYPE_INTEGER_VEC  = 1,

      TYPE_DOUBLE       = 2,
      TYPE_DOUBLE_VEC   = 3,
      TYPE_DOUBLE_MAT   = 4,

      TYPE_OBJECT       = 5,
      TYPE_OBJECT_VEC   = 6,

      TYPE_STRING       = 7,
      TYPE_STRING_VEC   = 8,

      TYPE_INTEGER64    = 9,
      TYPE_INTEGER64_VEC = 10,
    };


    //--------------------------------------------------------------------
    /// Returns class name of PowerFactory object.
    ///
    /// @return non-empty class name, e.g. ElmTerm, ElmCoup; never NULL
    //--------------------------------------------------------------------
    virtual const Value* GetClassName() const=0;


    //--------------------------------------------------------------------
    /// Returns the identifier number for the class of current object's
    /// instance.
    /// @see  api::Application::getClassId(const std::string& className)
    ///
    /// @return       class id > 0
    //--------------------------------------------------------------------
    virtual int GetClassId() const=0;


    //--------------------------------------------------------------------
    /// Returns the name of the object. Corresponds to attribute loc_name.
    ///
    /// @return   value of loc_name attribute, never NULL
    //--------------------------------------------------------------------
    virtual const Value* GetName() const=0;


    //--------------------------------------------------------------------
    /// Returns the full name (full hierarchy path) of the object.
    ///
    /// @return       full name, never NULL
    //--------------------------------------------------------------------
    virtual const Value* GetFullName(DataObject* relParent=0) const=0;


    //--------------------------------------------------------------------
    /// Returns collection of children for current object.
    ///
    /// @param[in]    recursive  =>  if false, only direct children of current
    ///                              object are returned; \n
    ///                              if true, additionally all children's children
    ///                              plus their children and so on... are returned
    ///
    /// @return       vector of DataObjects (vector<DataObject*>*), never NULL
    //--------------------------------------------------------------------
    virtual const Value* GetChildren(bool recursive, int classfilter=-1) const =0; //TODO CW: implement classfilter


    //--------------------------------------------------------------------
    /// Returns the parent object.
    ///
    /// @return      parent object; NULL, if on highest level
    //--------------------------------------------------------------------
    virtual DataObject* GetParent() const=0;


    //--------------------------------------------------------------------
    /// Checks whether object is a network data folder.
    ///
    /// @return      true for network data folder objects, e.g. IntBmu, IntZone...\n
    ///              false, otherwise
    //--------------------------------------------------------------------
    virtual bool IsNetworkDataFolder() const=0;


    //--------------------------------------------------------------------
    /// Checks whether the object is active (depending on currently
    /// activated variation stage)
    ///
    /// @return       true, if currently not active (added in inactive stage
    ///                     or deleted in currently active stage)\n
    ///               false, if object is currently active
    //--------------------------------------------------------------------
    virtual bool IsHidden() const=0;


    //--------------------------------------------------------------------
    /// Checks whether the object is deleted (stored in recycle bin)
    ///
    /// @return       true, if object is deleted and currently stored in
    ///                     recycle bin
    ///               false, otherwise
    //--------------------------------------------------------------------
    virtual bool IsDeleted() const =0;


    //--------------------------------------------------------------------
    /// Returns the information about the type of an attribute.
    ///
    /// @param[in]    attribute  =>  name of an attribute
    ///
    /// @return       type of the attribute \n
    ///               TYPE_INVALID on error (no attribute of that name exists)
    //--------------------------------------------------------------------
    virtual DataObject::AttributeType GetAttributeType(const char* attribute) const=0;


    //--------------------------------------------------------------------
    /// Returns the description of an attribute.
    ///
    /// @param[in]    attribute  =>  attribute name
    ///
    /// @return       NULL, if given attribute does not exist \n
    ///               description string
    ///               String must be released when no longer used.
    //--------------------------------------------------------------------
    virtual const Value* GetAttributeDescription(const char* attribute) const=0;


    //--------------------------------------------------------------------
    /// Returns the unit of an attribute, e.g. km, MW...
    ///
    /// @param[in]    attribute  =>  attribute name
    ///
    /// @return       NULL, if given attribute name does not exist \n
    ///               unit string, empty for attributes that have no unit \n
    ///               String must be released when no longer used.
    //--------------------------------------------------------------------
    virtual const Value* GetAttributeUnit(const char* attribute) const=0;

    virtual void GetAttributeSize(const char* attribute, int& countRows, int& countCols) const=0;

    virtual int GetAttributeInt(const char* attribute, int* error=0) const=0;
    virtual int GetAttributeInt(const char* attribute, int row, int col, int* error=0) const=0;

    virtual double GetAttributeDouble(const char* attribute, int* error=0) const=0;
    virtual double GetAttributeDouble(const char* attribute, int row, int col, int* error=0) const=0;

    virtual DataObject* GetAttributeObject(const char* attribute, int* error=0) const=0;
    virtual DataObject* GetAttributeObject(const char* attribute, int row, int* error=0) const=0;

    virtual void SetAttributeObject(const char* attribute, DataObject* obj, int* error=0) =0;
    virtual void SetAttributeObject(const char* attribute, DataObject* obj, int row, int* error=0) =0;

    virtual const Value* GetAttributeString(const char* attribute, int* error=0) const=0;
    virtual const Value* GetAttributeString(const char* attribute, int row, int* error=0) const=0;

    virtual void SetAttributeString(const char* attribute, const char* value, int* error=0) =0;
    virtual void SetAttributeString(const char* attribute, const char* value, int row, int col, int* error=0) =0;

    virtual void SetAttributeDouble(const char* attribute, double value, int* error=0) =0;
    virtual void SetAttributeDouble(const char* attribute, double value, int row, int col, int* error=0) =0;

    virtual void SetAttributeInt(const char* attribute, int value, int* error=0) =0;
    virtual void SetAttributeInt(const char* attribute, int value, int row, int col, int* error=0) =0;

    virtual void ResizeAttribute(const char* attribute, int rowSize, int colSize, int* error=0) =0;


    virtual DataObject* CreateObject(const char* className, const char* locname) =0;

    virtual void DeleteObject(int* error=0)=0;

    virtual const Value* Execute(const char* command, const Value* inArgs, int* error=0)=0;

    virtual void SetAttributes(const Value* values, int* error=0)=0;
    virtual const Value* GetAttributes(int* error=0) const=0;

    virtual DataObject* CreateObject(const char* classname, const Value* values, int* error=0)=0;

    //---------------------------------------------------------------------------------------
    /// Only required for manually committing changed values (performance optimization).
    /// @see SetWriteCacheEnabled()
    //---------------------------------------------------------------------------------------
    virtual void WriteChangesToDb() =0;
  }; //end class DataObject



  /// Wrapper for PowerFactory application.
  class DllExportImport Application
  {
  public:

    //--------------------------------------------------------------------
    /// Returns the version string for currently running PowerFactory,
    /// e.g. "14.0.505"
    ///
    /// @return string holding version information of PowerFactory application;
    ///                returned string is never null.\n
    ///                String must be released when no longer used.
    //--------------------------------------------------------------------
    virtual const Value* GetVersion() const=0;


    virtual const Value* GetTempDirectory() const=0;

    virtual const Value* GetWorkingDirectory() const=0;

    virtual const Value* GetInstallationDirectory() const=0;

    //--------------------------------------------------------------------
    /// Returns an instance of OutputWindow. Each api instance has one
    /// OutputWindow instance.
    ///
    /// @return instance of OutputWindow, never NULL
    //--------------------------------------------------------------------
    virtual OutputWindow* GetOutputWindow() =0;


    //--------------------------------------------------------------------
    /// Returns current user object.
    ///
    /// @return current user, not NULL. \n
    ///         Object must be released if no longer in use .
    //--------------------------------------------------------------------
    virtual DataObject* GetCurrentUser() const=0;


    //--------------------------------------------------------------------
    /// Returns currently active PowerFactory project.
    ///
    /// @return active project, NULL if is there no active project.\n
    ///         Object must be released if no longer in use .
    //--------------------------------------------------------------------
    virtual DataObject* GetActiveProject() const=0;

    //--------------------------------------------------------------------
    /// Returns currently active study case.
    ///
    /// @return active study case, NULL if there is no active case.\n
    ///         Object must be released if no longer in use.
    //--------------------------------------------------------------------
    virtual DataObject* GetActiveStudyCase() const=0;

    //--------------------------------------------------------------------
    /// Returns all objects that are currently relevant for calculation.
    /// This means, all lines, nodes, switches, transformers,... + types.
    ///
    /// @return objects relevant for calculation, never NULL. \n
    ///         Container must be released if no longer in use.
    //--------------------------------------------------------------------
    virtual const Value* GetCalcRelevantObjects() const=0;


    //--------------------------------------------------------------------
    /// Returns a class identifier number. Each class name corresponds to
    /// one unique number. The mapping of class name might be different
    /// for different build numbers of PowerFactory, but it is guaranteed
    /// that it will not changed while an Api instance exists.
    /// (Do not keep these numbers static, get them dynamically in your
    /// code using this method.)
    ///
    /// @param[in]    className  => PowerFactory class name
    ///
    /// @return       0, if given name is not a valid class name
    ///               >0, valid class id for given class
    //--------------------------------------------------------------------
    virtual int GetClassId(const char* className) const=0;


    //--------------------------------------------------------------------
    /// Returns a description for a PowerFactory class.
    ///
    /// @param[in]    className  =>  name of a PowerFactory class
    ///
    /// @return       description text, never NULL; but string is
    ///               empty for invalid class names
    //--------------------------------------------------------------------
    virtual const Value* GetClassDescription(const char* className) const=0;

    enum AttributeMode {
      MODE_DISPLAYED    = 0,
      MODE_INTERNAL     = 1
    };

    //---------------------------------------------------------------------------------------
    /// Changes the way how attribute values are accessed:
    ///     MODE_DISPLAYED = as displayed in PF (unit conversion applied)
    ///     MODE_INTERNAL  = as internally stored
    ///
    /// @author          Christoph Wolter
    ///
    /// @param[in]  mode  =>
    //---------------------------------------------------------------------------------------
    virtual void SetAttributeMode(AttributeMode mode)=0;
    virtual AttributeMode GetAttributeMode() const=0;

    //---------------------------------------------------------------------------------------
    /// Performance optimization: Internally in PF, all objects must be in a special edit
    /// mode before any value can be changed. Switching between this edit mode is quite
    /// time consuming. Setting this write cache option, each object will be set into
    /// edit mode only once and not automatically switched back. (They remain in edit mode.
    /// Optimized for consecutive value modifications.)
    /// The edit mode can be manually left by calling WriteChangesToDb()
    ///
    /// By default, this caching is disabled.
    ///
    /// @author          Christoph Wolter
    ///
    /// @param[in]  enabled  =>
    //---------------------------------------------------------------------------------------
    virtual void SetWriteCacheEnabled(bool enabled)=0;
    virtual bool IsWriteCacheEnabled() const=0;


    //--------------------------------------------------------------------
    /// Returns the information about the type of an attribute.
    ///
    /// @param[in]    attribute  =>  name of an attribute
    ///
    /// @return       type of the attribute \n
    ///               TYPE_INVALID on error (no attribute of that name exists)
    //--------------------------------------------------------------------
    virtual DataObject::AttributeType GetAttributeType(const char* classname, const char* attribute) const=0;


    //--------------------------------------------------------------------
    /// Returns the description of an attribute.
    ///
    /// @param[in]    attribute  =>  attribute name
    ///
    /// @return       NULL, if given attribute does not exist \n
    ///               description string
    ///               String must be released when no longer used.
    //--------------------------------------------------------------------
    virtual const Value* GetAttributeDescription(const char* classname, const char* attribute) const=0;


    //--------------------------------------------------------------------
    /// Returns the unit of an attribute, e.g. km, MW...
    ///
    /// @param[in]    attribute  =>  attribute name
    ///
    /// @return       NULL, if given attribute name does not exist \n
    ///               unit string, empty for attributes that have no unit \n
    ///               String must be released when no longer used.
    //--------------------------------------------------------------------
    virtual const Value* GetAttributeUnit(const char* classname, const char* attribute) const=0;


    virtual void GetAttributeSize(const char* classname, const char* attribute, int& countRows, int& countCols) const=0;

    virtual const Value* Execute(const char* command, const Value* inArgs, int* error=0) =0;


    virtual void DefineTransferAttributes(const char* classname, const char* attributes, int* error=0)=0;


    //---------------------------------------------------------------------------------------
    /// Writes all (in memory) modified objects to database.
    /// @see DataObject::WriteChangesToDb()
    //---------------------------------------------------------------------------------------
    virtual void WriteChangesToDb() =0;
  }; //end class Application


  /// Main Api class
  class DllExportImport Api
  {
  public:

    //--------------------------------------------------------------------
    /// Returns the version number of current api instance.
    ///
    /// @return version of api, e.g. 1.0
    //--------------------------------------------------------------------
    virtual const Value* GetVersion() const=0;


    //--------------------------------------------------------------------
    /// Releases an Api object instance.
    /// All Api object pointers created by the Api must be released
    /// using this function. Calling delete from an external DLL is not
    /// possible as the Api instance has an own memory management.
    ///
    /// @param[in]    pObject  =>  pointer to an
    ///
    /// @return 0 on success, \n
    ///         >0 on error
    //--------------------------------------------------------------------
    virtual int ReleaseObject(const DataObject* object) =0;


    //---------------------------------------------------------------------------------------
    /// Releases an Api Value object.
    /// Please note that if given object is of type Value::VECTOR, in addtion all contained
    /// Value objects are released too.
    ///
    /// @see also int ReleaseObject(const DataObject* object)
    ///
    /// @param[in]  object  =>  pointer to a Value object
    ///
    /// @return 0 on success, \n
    ///         >0 on error
    //---------------------------------------------------------------------------------------
    virtual int ReleaseValue(const Value* object) =0;


    //--------------------------------------------------------------------
    /// Returns an instance of the application. There exists one application
    /// object per Api instance. This application object must not be deleted.
    ///
    /// @return pointer to instance of application object \n
    ///         never NULL
    //--------------------------------------------------------------------
    virtual Application* GetApplication() =0;


    //---------------------------------------------------------------------------------------
    /// Returns true if PowerFactory is in debug mode. False otherwise.
    ///
    /// @return     true if PF is in debug mode, else false
    //---------------------------------------------------------------------------------------
    virtual bool IsDebug() const=0;

  protected:

    //--------------------------------------------------------------------
    /// Constructor.
    //--------------------------------------------------------------------
    Api(){};


    //--------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------
    virtual ~Api(){};

  }; //end of class Api

} //end namespace api

extern "C" {
  DllExportImport void DestroyApiInstance(api::Api*& api);
  DllExportImport api::Api* CreateApiInstance(const char* username, const char* password);
}

#endif // __API_HPP_INCLUDED
