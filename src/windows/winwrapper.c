#include <windows.h>
#include <shellapi.h>
#include <wchar.h>
#include "winwrapper.h"



// function to load packaged resources
void* LoadPackagedResource(unsigned long typeId, unsigned long resourceId, unsigned long* size)
{
    // Track results
    HRESULT hr = S_OK;

    // Resource management
    HRSRC resHandle = NULL;
    HGLOBAL dataHandle = NULL;
    void* rawBytes = NULL;
    DWORD fileSize = 0;

    // Locate the resource in the application's executable
    resHandle = FindResource(
       NULL,                 // this component
       (LPCSTR)(resourceId), // resource name
       (LPCSTR)(typeId)      // resource type
    );
    hr = (resHandle ? S_OK : E_FAIL);

    // Load the resource to the HGLOBAL
    if (SUCCEEDED(hr)){
       dataHandle = LoadResource(NULL, resHandle);
       hr = (dataHandle ? S_OK : E_FAIL);
    }
    
    // Lock the resource to retrieve memory pointer
    if (SUCCEEDED(hr)){
       rawBytes = LockResource(dataHandle);
       hr = (rawBytes ? S_OK : E_FAIL);
    }

    // Calculate the size
    if (SUCCEEDED(hr)){
       fileSize = SizeofResource(NULL, resHandle);
       hr = (fileSize ? S_OK : E_FAIL);
    }
    
    if (!SUCCEEDED(hr)) {
        //todo
    }
    
    // Return values of interest
    *size = fileSize;
    return rawBytes;
}

// function to open a URL in the user's preferred browser
void OpenURLHidden(char* url)
{
    ShellExecute(0, 0, url, 0, 0, SW_HIDE);
}
