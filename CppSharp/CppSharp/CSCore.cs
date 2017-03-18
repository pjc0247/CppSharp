using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;
using System.Threading.Tasks;

using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Emit;

namespace CppSharp
{
    public class CSCore
    {
        public static ExeContext LoadScripts(Assembly reference, Tuple<string, string>[] scripts)
        {
            Console.WriteLine("REF : {0}", reference.Location);

            var assembly = CreateAssembly(reference, scripts);

            

            return new ExeContext(assembly);
        }

        private static byte[] CreateAssembly(Assembly reference, Tuple<string, string>[] scripts)
        {
            var options = new CSharpParseOptions(LanguageVersion.CSharp6, DocumentationMode.Diagnose, SourceCodeKind.Script);
            string assemblyName = Path.GetRandomFileName();
            List<SyntaxTree> trees = new List<SyntaxTree>();
            List<MetadataReference> references = new List<MetadataReference>();

            // 지정 안해도 기본적으로 포함되어야 하는 레퍼런스들
            references.AddRange(
                new MetadataReference[]
                {
                MetadataReference.CreateFromFile(reference.Location),
                MetadataReference.CreateFromFile(typeof(Microsoft.CSharp.RuntimeBinder.RuntimeBinderException).Assembly.Location),
                MetadataReference.CreateFromFile(typeof(object).Assembly.Location),
                MetadataReference.CreateFromFile(typeof(Enumerable).Assembly.Location),
                MetadataReference.CreateFromFile(typeof(System.Text.RegularExpressions.Match).Assembly.Location),
                MetadataReference.CreateFromFile(typeof(System.Xml.XmlReader).Assembly.Location)
                //MetadataReference.CreateFromFile(Assembly.GetEntryAssembly().Location),
                });

            foreach (var script in scripts)
            {
                SyntaxTree syntaxTree = CSharpSyntaxTree.ParseText(script.Item2, options, path: script.Item1);

                trees.Add(syntaxTree);
            }

            var compilation = CSharpCompilation.Create(
                assemblyName,
                syntaxTrees: trees,
                references: references,
                options: new CSharpCompilationOptions(OutputKind.DynamicallyLinkedLibrary, scriptClassName: "MyClass"));

            using (var ms = new MemoryStream())
            {
                EmitResult result = compilation.Emit(ms);

                if (!result.Success)
                {
                    IEnumerable<Diagnostic> failures = result.Diagnostics
                    .Where(diagnostic =>
                    diagnostic.IsWarningAsError ||
                    diagnostic.Severity == DiagnosticSeverity.Error);

                    // ii
                    foreach (Diagnostic diagnostic in failures)
                    {
                        var ln = diagnostic.Location.GetLineSpan();

                        Console.WriteLine(diagnostic.GetMessage());
                    }

                    var firstError = failures.First();
                    throw new CompileException(
                        firstError.Id, firstError.GetMessage(), firstError);
                }
                else
                {
                    ms.Seek(0, SeekOrigin.Begin);

                    return ms.ToArray();
                }
            }
        }
    }
}
