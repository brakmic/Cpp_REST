## JSON Clients based on C++ REST SDK & C#

This demo consists of three small VS projects:

a) **C++ DLL with exported functions**

b) **C++ Console App** using headers from DLL

c) **C# Console App** using DllImport

The DLL Exports are as follows:

<img src="http://p28.imgup.net/exports617e.png" />

**C++ Client App**

The C++ App uses the *Connector.h* header from DLL to create a *unique_ptr*
of the Connector class. 

Connector utilizes internally the <a href="https://casablanca.codeplex.com/" target="_blank">C++ REST SDK (Casablanca)</a> to call a
publicly available JSON Echo service.

<img src="http://p33.imgup.net/call_echo_c916.png"/>

To make this example a little bit more realistic the method **GetJson** 
expects a function object. This object (a function pointer) is used to
display the JSON response. There's also a possibility to send additional
headers.

Inside the **GetJson** method the server communication is handled asynchronously
via objects & functions from the <a href="https://msdn.microsoft.com/de-de/library/jj987780.aspx" target="_blank">pplx</a> namespace.

**C# Client App**

The .NET App imposed a several restrictions to the exported functions from the
C++ library. Instead of C++ <a href="http://en.cppreference.com/w/cpp/utility/functional/function" target="_blank">function templates</a> I had to use old-style C function
pointers. This is because .NET can only marshal Delegates to function pointers. 
The same happened to the C++ string class. It was replaced by 
raw *wchar_t* pointers. Ugly, but playing with .NET *DllImport* is always rather
unpleasant.

This is how the whole <a href="https://msdn.microsoft.com/en-us/library/system.runtime.interopservices.dllimportattribute(v=vs.110).aspx" target="_blank">DllImport soup</a> looks like:

<img src="http://p82.imgup.net/csharp_clic7a7.png" />

First, we declare a delegate and decorate it as an "unmanaged" (that is, non-NET callable) function pointer. Also, we set the
calling convention to StdCall. This means that the callee will be responsible to clean the stack and not the caller. More info
on argument passing and calling conventions <a href="https://msdn.microsoft.com/en-us/library/984x0h58.aspx" target="_blank">here</a>

Second, we grab the DLL which hopefully exports the desired function. In this case the DLL *Connector.dll* should be located in the directory of
the executing assembly. In other cases we'd adjust the first argument after [DllImport] Attribute. Also, the same calling convention is applied.
Now, it's important to know that functions from non-NET DLLs are statically imported externals and very often demand special treatment of passed arguments.
In this case we have to marshal a pure .NET entity, the Delegate, to a raw C function pointer. To achieve this we use another attribute [MarshalAs] and 
decorate the *OnCompletedDelegate* as an "unmanage function pointer".

Now the C++ DLL will be able to "call back" our delegate as soon as the JSON Echo Server delivers a response.

*Example, C++ client calling service & passing additional headers*

<img src="http://w74.imgup.net/c_client6739.png" />

*Example, C# client calling service*

<img src="http://p68.imgup.net/csharp_clie7d9.png" />

**IMPORTANT**

The clients need these two libraries to be located in their root folders:

*Connector.dll*

*cpprest120d_2_4.dll* (this is the library from C++ REST SDK and can be installed via <a href="https://www.nuget.org/packages/cpprestsdk/" target="_blank">Nuget</a>)

**LICENSE**

MIT


