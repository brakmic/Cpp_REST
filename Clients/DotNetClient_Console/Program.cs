using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace DotNetClient_Console
{
    [UnmanagedFunctionPointer(CallingConvention.StdCall, 
                              CharSet=CharSet.Unicode)]
    public delegate void OnCompletedDelegate(string json);

    class Program
    {
        [DllImport(@"Connector.dll",
        EntryPoint = "GetJson",
        CallingConvention = CallingConvention.StdCall, 
                            CharSet=CharSet.Unicode)]
        private static extern void GetJson(string webServerUrl, [MarshalAs(UnmanagedType.FunctionPtr)] OnCompletedDelegate dlg);

        static void Main(string[] args)
        {
            string url = "http://date.jsontest.com/";
            OnCompletedDelegate del = new OnCompletedDelegate(OnCompleted);
            GetJson(url, del);
            Console.ReadLine();
        }

        static void OnCompleted(string json)
        {
            Console.WriteLine(json);
        }
    }
}
