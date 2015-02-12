// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CONNECTOR_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CONNECTOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CONNECTOR_EXPORTS
#define CONNECTOR_API __declspec(dllexport)
#else
#define CONNECTOR_API __declspec(dllimport)
#endif

//a simple Tracer
#ifdef ENABLE_TRACE
bool _trace(TCHAR *format, ...)
{
    TCHAR buffer[2048];

    va_list argptr;
    va_start(argptr, format);
    wvsprintf(buffer, format, argptr);
    va_end(argptr);

    OutputDebugString(buffer);

    return true;
}
#endif

namespace app
{
    //we use a C-style typedef to make the mapping of .NET delegates
    //easier (possible at all?)
    typedef void(__stdcall * on_completed)(const wchar_t*);

    //HTTP Header struct
    //here we use wchar_t* instead of wstring for easier marshalling of 
    //.NET strings
    struct CONNECTOR_API Header
    {
        const wchar_t* Key;
        const wchar_t* Value;
    };

    //implementation
    class CONNECTOR_API Connector
    {
    public:
        //Ctor
        Connector(void);
        //Get a JSON response from a web server. 
        //The caller has to provide a function callback which will be
        //utilized to return server responses
        void GetJson(const std::wstring& webServerUrl,
            const std::function<void(const wchar_t*)>& on_completed) const;
        //Get a JSON response from a web werver.
        //This function accepts a vector of Headers
        //Also, the caller must provide a proper callback for reading the 
        //server results
        void GetJson(const std::wstring& webServerUrl,
            const std::vector<Header>& headers,
            const std::function<void(const wchar_t*)>& on_completed) const;
        //Dtor
        virtual ~Connector();
    };

}

extern "C"
{
    //Get a JSON response from a web server
    void CONNECTOR_API __stdcall GetJson(const wchar_t* webServerUrl, app::on_completed callback);
    //Get a JSON response from a web server
    void CONNECTOR_API __stdcall GetJsonWithHeaders(const wchar_t* webServerUrl,
                               const app::Header headers[], 
                               app::on_completed callback);

}
