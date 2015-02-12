// HtppClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
using namespace app;

namespace client
{
    void on_completed(const wchar_t* result)
    {
        wcout << result << endl;
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    HINSTANCE hInst = LoadLibrary(_T("Connector.dll"));
    if(hInst == nullptr)
    {
        wcout << "Could not find Library!" << endl;
        return 1;
    }
    
    vector<Header> headers = {
        { L"key_1", L"val_1" },
        { L"key_2", L"val_2" },
        { L"key_3", L"val_3" },
        { L"key_4", L"val_4" }
    };
    function<void(const wchar_t*)> callback = client::on_completed;
    const wstring webServer = L"http://date.jsontest.com/";
    unique_ptr<Connector> client = make_unique<Connector>();
    client->GetJson(webServer, headers, client::on_completed);
    system("PAUSE");
	return 0;
}

