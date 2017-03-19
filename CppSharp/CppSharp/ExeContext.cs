using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace CppSharp
{
    [Serializable]
    public class ExeContext : IDisposable
    {
        private AppDomain ad { get; set; }
        private Assembly assembly { get; set; }

        private byte[] rawAssembly { get; set; }

        public ExeContext(byte[] rawAssembly)
        {
            this.rawAssembly = rawAssembly;

            this.ad = AppDomain.CreateDomain("MyExeContext" + DateTime.Now.ToShortTimeString(),
            AppDomain.CurrentDomain.Evidence,
            AppDomain.CurrentDomain.SetupInformation);
            AppDomain.CurrentDomain.AssemblyResolve += OnAssemblyResolve;

            this.ad.UnhandledException += Ad_UnhandledException;
            this.assembly = ad.Load(rawAssembly);
        }

        private void Ad_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            Console.WriteLine(e);
        }

        // 현재 앱 도메인에 올릴 때,
        // 파일에서 찾지 말고 똑같이 메모리에서 올리게 한다.
        private Assembly OnAssemblyResolve(object sender, ResolveEventArgs args)
        {
            AppDomain.CurrentDomain.AssemblyResolve -= OnAssemblyResolve;
            return Assembly.Load(rawAssembly);
        }

        public ExeProxy GetProxy()
        {
            Type type = assembly.GetType("MyClass");
            object obj = Activator.CreateInstance(type);

            return new ExeProxy(obj);
        }
        public void Dispose()
        {
            //AppDomain.Unload(ad);
            ad = null;
            assembly = null;
            rawAssembly = null;
        }
    }
}