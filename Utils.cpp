#include "Utils.hpp"

#include <windows.h>

using std::vector;
using std::string;

extern "C" {
typedef api::Api* (__cdecl *CREATEAPI)(const char* username, const char* password);
typedef void (__cdecl *DESTROYAPI)(api::Api*&);
}

#undef GetClassName

HINSTANCE dllHandle = NULL;

api::Api* ApiFixture::instance = 0;

ApiFixture::ApiFixture()
{
  //create an api instance

    dllHandle = LoadLibraryExA("C:\\DIgSILENT\\pf150\\digapi.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

  if (!dllHandle) {
    return;
  }

  CREATEAPI createApi = (CREATEAPI) GetProcAddress ((struct HINSTANCE__*)dllHandle, "CreateApiInstance");
  std::cout << "Creating API instance..." << std::endl;
  instance = createApi(NULL, NULL);
  if (instance) {
    std::cout << "API instance successfully created." << std::endl;
  }
}

ApiFixture::~ApiFixture()
{
  //note: destroying the api instance will immediately terminate the whole process as this is the way
  //      how PowerFactory is closed
  if (dllHandle) {
    std::cout << "Releasing API instance..." << std::endl;
    DESTROYAPI destroyApi = (DESTROYAPI) GetProcAddress ((struct HINSTANCE__*)dllHandle, "DestroyApiInstance");
    destroyApi(instance);
    instance = NULL;

    FreeLibrary(dllHandle);
    dllHandle = NULL;
  }
}

ValueGuard::~ValueGuard()
{
  if (ApiFixture::GetInstance() && m_val) {
    ApiFixture::GetInstance()->ReleaseValue(m_val);
  }
}


std::vector<api::DataObject*> Utils::GetChildren(api::DataObject* parent, std::string classNameFilter)
{
  vector<DataObject*> result;

  ValueGuard children(parent->GetChildren(true));
  for (size_t i=0, count = children->VecGetSize(); i < count; ++i) {
    api::DataObject* child = children->VecGetDataObject(i);
    if (classNameFilter.empty()) {
      result.push_back(child);
    }
    else {
      ValueGuard classname(child->GetClassName());
	  ValueGuard valuename(child->GetName());
		//std::cout << classname->GetString() 
		//	<< "::" << valuename->GetString() << std::endl;
      if (strcmp(classname->GetString(), classNameFilter.c_str()) == 0) {
        result.push_back(child);
      }
    }
  }

  return result;
}
