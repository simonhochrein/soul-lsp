#include <fstream>
#include <iostream>
#include "Parser.hpp"

//struct Definition
//{
//    std::string name;
//    CodeLocation location;
//};
//
//struct Scope {
//    CodeLocation start;
//    CodeLocation end;
//    Scope* parent;
//
//    std::vector<Scope*> childScopes;
//    std::map<std::string, Definition> identifiers;
//
//    void search(std::string name);
//};
//
//void Scope::search(std::string name) {
//    if (identifiers.find(name) != identifiers.end()) {
//        std::cout << identifiers[name].location.getSourceLine();
//    }
//    else {
//        if (parent != nullptr) {
//            parent->search(name);
//        }
//    }
//}
//
//Scope root;
//Scope* currentScope = &root;
//
//void startScope(int *depth, SimpleTokeniser &tokenizer) {
//    for (int i = 0; i < *depth; i++)
//        std::cout << " ";
//    auto loc = tokenizer.location.getLineAndColumn();
//    std::cout << "Begin Scope " << " " << loc.line << " " << loc.column << std::endl;
//    (*depth)++;
//    
//    Scope *scope = new Scope;
//    scope->start = tokenizer.location;
//    scope->parent = currentScope;
//    currentScope->childScopes.push_back(scope);
//    currentScope = scope;
//}
//
//void endScope(int *depth, SimpleTokeniser& tokenizer) {
//    (*depth)--;
//    for (int i = 0; i < *depth; i++)
//        std::cout << " ";
//
//    auto loc = tokenizer.location.getLineAndColumn();
//    std::cout << "End Scope " << " " << loc.line << " " << loc.column << std::endl;
//
//    currentScope->end = tokenizer.location;
//    currentScope = currentScope->parent;
//}
//
//void findScopes(SourceFile &f) {
//    int depth = 0;
//    SimpleTokeniser tokenizer(CodeLocation::createFromSourceFile(f), true);
//
//    while (!tokenizer.matches(Token::eof)) {
//        if (tokenizer.matches(Keyword::for_) || tokenizer.matches(Keyword::if_) || tokenizer.matches(Keyword::loop)) {
//            //tokenizer.skip();
//            auto ret = tokenizer.skipPastMatchingCloseDelimiter(Operator::openParen, Operator::closeParen);
//            if (tokenizer.matches(Operator::openBrace)) {
//                startScope(&depth, tokenizer);
//            }
//            else {
//                startScope(&depth, tokenizer);
//                while (!tokenizer.matches(Token::eof) && !tokenizer.matches(Operator::semicolon)) {
//                    tokenizer.skip();
//                }
//                endScope(&depth, tokenizer);
//            }
//        }
//        else if (tokenizer.matches(Operator::openBrace)) {
//            startScope(&depth, tokenizer);
//        } else if (tokenizer.matches(Operator::closeBrace)) {
//            endScope(&depth, tokenizer);
//        }
//
//        if (tokenizer.matches(Keyword::let)) {
//            tokenizer.skip();
//            std::string name = choc::text::trim(tokenizer.currentStringValue);
//            if (currentScope->identifiers.find(name) == currentScope->identifiers.end()) {
//                currentScope->identifiers[name] = { name, tokenizer.location };
//            }
//        }
//
//        /*if (tokenizer.matches(Token::identifier)) {
//            std::string name = choc::text::trim(tokenizer.currentStringValue);
//            if (currentScope->identifiers.find(name) == currentScope->identifiers.end()) {
//                currentScope->identifiers[name] = {name, tokenizer.location};
//            }
//        }*/
//        tokenizer.skip();
//    }
//}
//
//bool locationGT(CodeLocation a, CodeLocation b) {
//    return a.getByteOffsetInFile() > b.getByteOffsetInFile();
//}
//bool locationLT(CodeLocation a, CodeLocation b) {
//    return a.getByteOffsetInFile() < b.getByteOffsetInFile();
//}
//
//Scope findScope(Scope &scope, CodeLocation location) {
//    for (auto s : scope.childScopes) {
//        std::cout << (locationGT(location, s->start) ? 1 : 0) << " " << (locationLT(location, s->end) ? 1 : 0) << std::endl;
//        if (locationGT(location, s->start) && locationLT(location, s->end)) {
//            return findScope(*s, location);
//        }
//    }
//    if (locationGT(location, scope.start) && locationLT(location, scope.end)) {
//        return scope;
//    }
//}
//
//struct ModuleIndexer : ASTVisitor {
//    void visitObject(AST::ModuleBase& m) {
//        ASTVisitor::visitObject(m);
//
//        if (m.objectType == AST::ObjectType::Processor) {
//            auto proc = m.getAsProcessor();
//            std::cout << proc->name.toString() << std::endl;
//            std::cout << "Endpoints: " << std::endl;
//            for (auto endpoint : proc->getEndpoints()) {
//                std::cout << "  " << endpoint->name.toString() << std::endl;
//            }
//            std::cout << "Structures: " << std::endl;
//            for (auto structure : proc->getStructDeclarations()) {
//                std::cout << "  " << structure->name.toString() << std::endl;
//            }
//            std::cout << "Variables: " << std::endl;
//            for (auto variable : proc->getStateVariableList()) {
//                std::cout << "  " << variable->name.toString() << std::endl;
//            }
//            std::cout << "Functions: " << std::endl;
//            for (auto function : *proc->getFunctionList()) {
//                std::cout << "  " << function->name.toString() << std::endl;
//            }
//        }
//    }
//};
//
//int main(int argc, char **argv)
//{
//    std::cout << argv[1] << std::endl;
//    std::ifstream input(argv[1], std::ios::in);
//    std::string inputText;
//    do
//    {
//        inputText += input.get();
//    } while (!input.eof());
//    inputText[inputText.size() - 1] = 0;
//
//    soul::AST::Allocator allocator;
//    soul::pool_ptr<soul::AST::Namespace> topLevelNamespace = soul::AST::createRootNamespace(allocator);
//    auto sourceFile = soul::SourceFile{"ringtone.soul", inputText};
//    soul::CodeLocation location = soul::CodeLocation::createFromSourceFile(sourceFile);
//
//    CompileMessageList errors;
//    CompileMessageHandler handler(errors);
//    soul::messageHandler = &handler;
//    try
//    {
//        auto ast = soul::Compiler::parseTopLevelDeclarations(allocator, location, *topLevelNamespace);
//
//        struct Visitor : ASTVisitor {
//            void visit(AST::VariableDeclaration& v) override
//            {
//                ASTVisitor::visit(v);
//                std::cout << v.context.location.getLineAndColumn().column << v.name.toString() << std::endl;
//            }
//            void visit(AST::VariableRef& r) override {
//                ASTVisitor::visit(r);
//                auto start = r.context.location.getLineAndColumn();
//                std::cout << start.line << ":" << start.column << ":" << r.variable->name.toString() << std::endl;
//            }
//            void visit(AST::QualifiedIdentifier& i) override {
//                ASTVisitor::visit(i);
//                auto range = SourceCodeUtilities::findRangeOfASTObject(i);
//                auto start = i.context.location.getLineAndColumn();
//                std::cout << start.line << ":" << start.column << ":";
//                for (auto path : i.pathSections) {
//
//                }
//                std::cout << std::endl;
//            }
//            void visitObject(AST::ModuleBase& m) {
//                ASTVisitor::visitObject(m);
//            }
//            void visitObject(AST::Expression& expression) override {
//                ASTVisitor::visitObject(expression);
//                /*if (expression.kind == AST::ExpressionKind::type) {
//                    std::cout << "Type" << std::endl;
//                    std::cout << expression.context.location.getSourceLine() << std::endl;
//                    std::cout << expression.getConcreteType()->getDescription() << std::endl;
//                    std::cout << expression.context.location.getLineAndColumn().column << std::endl;
//                }
//                if (expression.kind == AST::ExpressionKind::unknown) {
//                    std::cout << "Unknown" << std::endl;
//                    std::cout << expression.context.location.getSourceLine() << std::endl;
//                    std::cout << expression.context.location.getLineAndColumn().column << std::endl;
//                }
//                if (expression.kind == AST::ExpressionKind::value) {
//                    std::cout << "Value" << std::endl;
//                    std::cout << expression.context.location.getSourceLine() << std::endl;
//                    std::cout << expression.context.location.getLineAndColumn().column << std::endl;
//                }*/
//            }
//        };
//
//        findScopes(sourceFile);
//        auto scope= findScope(root, location.getOffset(32, 0));
//
//        scope.search("phaseIncrement");
//        //std::cout << scope.start.getSourceLine() << std::endl << "===" << std::endl << scope.end.getSourceLine() << std::endl;
//
//        //Visitor().visitObject(ast[0]);
//        /*for(auto mod : ast) {
//            ModuleIndexer().visitObject(mod);
//        }*/
//
//        //SimpleTokeniser tokenizer(location, true);
//        //auto loc = location.getOffset(25, 20);
//        //auto lastPos = tokenizer.startLocation;
//        //std::string_view last;
//        //TokenType type;
//
//        //Scope root;
//
//        //Scope* current = &root;
//
//        //while (!tokenizer.matches(Token::eof))
//        //{
//        //    type = tokenizer.currentType;
//        //    lastPos = tokenizer.location;
//        //    tokenizer.skip();
//
//        //    if (type == Token::identifier) {
//        //        std::string_view str(
//        //            lastPos.location.getAddress(), tokenizer.location.location.getAddress() - lastPos.location.getAddress());
//        //        //std::cout << choc::text::trim(str) << std::endl;
//        //        current->identifiers.push_back(Definition{ choc::text::trim(str) });
//        //    }
//        //    if (type == "{") {
//        //        Scope* scope = new Scope{};
//        //        scope->start = lastPos;
//        //        scope->parent = current;
//        //        current->childScopes.push_back(scope);
//
//        //        current = scope;
//        //        //std::cout << "New Scope" << std::endl;
//        //    }
//        //    else if (type == "}") {
//        //        current->end = lastPos;
//        //        current = current->parent;
//        //        //std::cout << "end scope" << std::endl;
//        //    }
//        //   /* if (tokenizer.currentType == Token::identifier || tokenizer.currentType == Token::blockIdentifier)
//        //    {
//        //        break;
//        //    }*/
//        //}
//        //printScope(root, 0);
//    }
//    catch (AbortCompilationException ex)
//    {
//        std::cout << errors.toString() << std::endl;
//    }
//}

int main(int argc, char** argv)
{
}