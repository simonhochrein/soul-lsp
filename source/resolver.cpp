#include "SOUL/source/modules/soul_core/soul_core.cpp"
#include <fstream>
#include <iostream>
#include <vector>

using namespace soul;

struct Definition
{
    std::string name;
    CodeLocation start;
};

int main()
{
    std::ifstream input("../test/ringtone.soul", std::ios::in);
    std::string inputText;
    do
    {
        inputText += input.get();
    } while (!input.eof());
    inputText[inputText.size() - 1] = 0;

    soul::AST::Allocator allocator;
    soul::pool_ptr<soul::AST::Namespace> topLevelNamespace = soul::AST::createRootNamespace(allocator);
    auto sourceFile = soul::SourceFile{"ringtone.soul", inputText};
    soul::CodeLocation location = soul::CodeLocation::createFromSourceFile(sourceFile);

    CompileMessageList errors;
    CompileMessageHandler handler(errors);
    soul::messageHandler = &handler;
    try
    {
        // auto ast = soul::Compiler::parseTopLevelDeclarations(allocator, location, *topLevelNamespace);

        // struct Visitor : public ASTVisitor
        // {
        // public:
        //     void visitObject(AST::ModuleBase &base)
        //     {
        //         ASTVisitor::visitObject(base);
        //     }
        //     void visit(AST::Block &b)
        //     {
        //         ASTVisitor::visit(b);
        //         auto range = SourceCodeUtilities::iterateSyntaxTokens(b);
        //         auto start = range.start.getLineAndColumn();
        //         auto end = range.end.getLineAndColumn();
        //         std::cout << start.line << " " << start.column << std::endl;
        //         std::cout << end.line << " " << end.column << std::endl;
        //     }

        //     std::vector<Definition> definitions;
        //     std::vector<Definition> references;
        //     pool_ptr<CodeLocation> location;
        // };

        // Visitor test;
        // test.location = location;
        // test.visitObject(ast[0].get());

        SimpleTokeniser tokenizer(location, true);
        auto loc = location.getOffset(25, 20);
        auto lastPos = tokenizer.startLocation.location;
        std::string_view last;
        TokenType type;

        while (!tokenizer.matches(Token::eof))
        {
            auto start = tokenizer.location.getLineAndColumn();
            if (start.line >= 11 && start.column > 7)
            {
                std::cout << type.getDescription() << std::endl;
                std::string_view str(
                    lastPos.getAddress(), tokenizer.location.location.getAddress() - lastPos.getAddress());
                std::cout << str << std::endl;
                break;
            }
            type = tokenizer.currentType;
            lastPos = tokenizer.location.location;
            tokenizer.skip();
        }

        // SourceCodeUtilities::findASTObjectAt(*ast[0].get().getAsBlock(), location.getOffset(25, 20));
        // for (auto fn : ast[0].get().getFunctions())
        // {
        //     // auto obj = SourceCodeUtilities::findASTObjectAt(fn.get(), location.getOffset(25, 20));
        // }

        // for (auto def : test.definitions)
        // {
        //     auto start = def.start.getLineAndColumn();
        //     std::cout << "D" << start.line << ":" << start.column << "-" << start.column + def.name.size() << ":" << def.name << std::endl;
        // }

        // for (auto def : test.references)
        // {
        //     auto start = def.start.getLineAndColumn();
        //     std::cout << "R" << start.line << ":" << start.column << "-" << start.column + def.name.size() << ":" << def.name << std::endl;
        // }

        // if (found != nullptr)
        // {
        // }
        // else
        // {
        //     std::cout << "No results found" << std::endl;
        // }
    }
    catch (AbortCompilationException ex)
    {
        std::cout << errors.toString() << std::endl;
    }
}