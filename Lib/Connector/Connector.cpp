// Connector.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Connector.h"


using namespace std;
using namespace concurrency;
using namespace concurrency::streams;
using namespace utility;                                        // Common utilities like string conversions
using namespace web;                                            // Common features like URIs.
using namespace web::http;                                      // Common HTTP functionality
using namespace web::http::client;                              // HTTP client features
using namespace concurrency::streams;                           // Asynchronous streams
using namespace web::http::experimental::listener;              // HTTP server
using namespace web::experimental::web_sockets::client;         // WebSockets client
using namespace web::json;                                      // JSON library
using namespace app;

Connector::Connector()
{
    wcout << "constructor called" << endl;
}

Connector::~Connector()
{
    wcout << "destructor called" << endl;
}

//Get a JSON response from web server
void Connector::GetJson(const wstring& webServerUrl, 
                        const function<void(const wchar_t*)>& on_completed) const
{
    _trace(L"%S %S", L"Connecting web server ", webServerUrl);
    uri _uri(webServerUrl);
    http_client client(_uri);

    client.request(methods::GET).then([=](http_response response) -> pplx::task<json::value>
    {
        if(response.status_code() == status_codes::OK)
        {
            wcout << "got response" << endl;
            return response.extract_json();
        }
        // Handle error cases, for now return empty json value...
        return pplx::task_from_result(json::value());
    }).then([=](pplx::task<json::value> previousTask)
    {
        wcout << "extracting json" << endl;
        const json::value& val = previousTask.get();
        const wstring out = val.serialize();
        if(on_completed != nullptr)
        {
            on_completed(out.c_str());
        }
        else
        {
            wcout << "no callback to return JSON" << endl;
        }
    });
}
//Get JSON response from web server
void Connector::GetJson(const wstring& webServerUrl, const vector<Header>& headers,
                                    const function<void(const wchar_t*)>& on_completed) const
{
    _trace(L"%S %S", L"Connecting web server ", webServerUrl);
    uri _uri(webServerUrl);
    http_client client(_uri);
    http_request request(methods::GET);
    //copy headers
    for(Header h : headers)
    {
        wcout << "Header Key: " << h.Key << ", Value: " << h.Value << endl;
        request.headers().add(h.Key, h.Value);
    }

    //execute request
    client.request(request).then([=](http_response response) -> pplx::task<json::value>
    {
        if(response.status_code() == status_codes::OK)
        {
            return response.extract_json();
        }
        // Handle error cases, for now return empty json value...
        return pplx::task_from_result(json::value());
    }).then([=](pplx::task<json::value> previousTask)
    {
        const json::value& val = previousTask.get();
        const wstring out = val.serialize();
        if(on_completed != nullptr)
        {
            on_completed(out.c_str());
        }
        else
        {
            wcout << "no callback to return JSON" << endl;
        }
    });
}

extern "C" 
{

    void CONNECTOR_API __stdcall GetJson(const wchar_t* webServerUrl, on_completed callback)
    {
        Connector connector;
        const wstring _url(webServerUrl);
        auto _fun = function<void(const wchar_t*)>(callback);
        connector.GetJson(_url, _fun);
    }

    void CONNECTOR_API __stdcall GetJsonWithHeaders(const wchar_t* webServerUrl, const Header headers[], on_completed callback)
    {
        Connector connector;
        const wstring _url(webServerUrl);
        auto _fun = function<void(const wchar_t*)>(callback);
        const vector<Header> _headers(headers, headers + sizeof headers / sizeof headers[0]);
        connector.GetJson(_url, _headers, _fun);
    }
}
