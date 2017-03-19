﻿using Microsoft.CodeAnalysis;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CppSharp
{
    public class CompileException : Exception
    {
        public string code { get; private set; }
        public string message { get; private set; }
        public Diagnostic diagnostic { get; private set; }

        public CompileException(string code, string message, Diagnostic diagnostic)
        {
            this.code = code;
            this.message = message;
            this.diagnostic = diagnostic;
        }
    }
}