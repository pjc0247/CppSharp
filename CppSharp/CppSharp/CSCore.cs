using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;
using System.Threading.Tasks;

using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Emit;

namespace CppSharp
{
    public class CSCore
    {
        private static List<SyntaxTree> syntaxTrees = new List<SyntaxTree>();

        public static ExeContext LoadScripts(Assembly[] references, Tuple<string, string>[] scripts)
        {
            var assembly = CreateAssembly(references, scripts);

            return new ExeContext(assembly);
        }

        public static void RegisterStruct(string name)
        {
            var options = new CSharpParseOptions(LanguageVersion.CSharp6, DocumentationMode.Diagnose, SourceCodeKind.Script);

            var structSyntax = SyntaxFactory.StructDeclaration(name)
                .AddModifiers(SyntaxFactory.Token(SyntaxKind.PublicKeyword));
                
            //SyntaxFactory.VariableDeclaration()
        }
        public static void RegisterFunction(string name, Type returnType, Type[] paramTypes)
        {
            var options = new CSharpParseOptions(LanguageVersion.CSharp6, DocumentationMode.Diagnose, SourceCodeKind.Script);

            // 귀찮
            /*
             *            var classId = SyntaxFactory.IdentifierName("CSFunctionBindings");
            var methodId = SyntaxFactory.IdentifierName("Invoke");

            var arguments = new List<ArgumentSyntax>();
            foreach (var paramType in paramTypes)
            {
                //SyntaxFactory.expression
                arguments.Add(
                    SyntaxFactory.Argument(
                        SyntaxFactory.(
                            SyntaxKind.literal,
                            SyntaxFactory.Literal("A"))));
            }*/
            /*
            var memberaccess = SyntaxFactory.MemberAccessExpression(SyntaxKind.SimpleMemberAccessExpression, classId, methodId); 
            var body = SyntaxFactory.Block(
                SyntaxFactory.ExpressionStatement(
                    SyntaxFactory.InvocationExpression(memberaccess, )
                ));
                */
            var paramString = "";
            int cnt = 0;
            foreach (var paramType in paramTypes)
            {
                paramString += $"_p{cnt},";
                cnt++;
            }
            paramString = paramString.Substring(0, paramString.Length - 1);

            var statement = $"return ({returnType.FullName})CSFunctionBindings.Invoke(\"{name}\", new object[] {{ {paramString} }});";
            var body = SyntaxFactory.Block(
                SyntaxFactory.ParseStatement(statement));

            var parameters = new List<ParameterSyntax>();
            cnt = 0;
            foreach (var paramType in paramTypes)
            {
                var p = SyntaxFactory.Parameter(
                    SyntaxFactory.Identifier($"_p{cnt}"))
                    .WithType(SyntaxFactory.ParseTypeName(paramType.FullName));

                parameters.Add(p);

                cnt++;
            }

            var syntax = SyntaxFactory.ParseStatement("System.Console.WriteLine(1234);");
            var method = SyntaxFactory.MethodDeclaration(SyntaxFactory.ParseTypeName(returnType.FullName), name)
                .AddModifiers(SyntaxFactory.Token(SyntaxKind.PublicKeyword))
                .WithParameterList(SyntaxFactory.ParameterList(
                    SyntaxFactory.SeparatedList(parameters.ToArray())))
                .WithBody(body);

            var tree = SyntaxFactory.SyntaxTree(SyntaxFactory.CompilationUnit().AddMembers(method), options);

            syntaxTrees.Add(tree);
        }

        private static byte[] CreateAssembly(Assembly[] additionalReferences, Tuple<string, string>[] scripts)
        {
            var options = new CSharpParseOptions(LanguageVersion.CSharp6, DocumentationMode.Diagnose, SourceCodeKind.Script);
            var assemblyName = Path.GetRandomFileName();
            var references = new List<MetadataReference>();

            // 지정 안해도 기본적으로 포함되어야 하는 레퍼런스들
            references.AddRange(
                new MetadataReference[]
                {
                MetadataReference.CreateFromFile(typeof(Microsoft.CSharp.RuntimeBinder.RuntimeBinderException).Assembly.Location),
                MetadataReference.CreateFromFile(typeof(object).Assembly.Location),
                MetadataReference.CreateFromFile(typeof(Enumerable).Assembly.Location),
                MetadataReference.CreateFromFile(typeof(System.Text.RegularExpressions.Match).Assembly.Location),
                MetadataReference.CreateFromFile(typeof(System.Xml.XmlReader).Assembly.Location)
                //MetadataReference.CreateFromFile(Assembly.GetEntryAssembly().Location),
                });

            foreach (var reference in additionalReferences)
                references.Add(MetadataReference.CreateFromFile(reference.Location));

            foreach (var script in scripts)
            {
                SyntaxTree syntaxTree = CSharpSyntaxTree.ParseText(script.Item2, options, path: script.Item1);

                syntaxTrees.Add(syntaxTree);
            }

            var compilation = CSharpCompilation.Create(
                assemblyName,
                syntaxTrees: syntaxTrees,
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
