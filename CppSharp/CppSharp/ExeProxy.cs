using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.Threading.Tasks;

namespace CppSharp
{
    public class ExeProxy
    {
        public object target { get; private set; }
        public Type innerType { get; private set; }

        public ExeProxy(object target)
        {
            this.target = target;
            this.innerType = target.GetType();
        }

        public object Invoke(string name, object[] args)
        {
            return innerType.InvokeMember(
                name,
                BindingFlags.Public | BindingFlags.Instance | BindingFlags.InvokeMethod,
                Type.DefaultBinder, target, args);
        }
    }
}
