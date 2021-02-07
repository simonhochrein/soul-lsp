#include "Server.hpp"
#include "SOUL/source/modules/soul_core/soul_core.cpp"

struct ServerFile
{
    soul::SourceFile file;
    std::string text;
    std::string uri;
};

enum CompletionItemKind {
	Text = 1,
	Method = 2,
	Function = 3,
	Constructor = 4,
	Field = 5,
	Variable = 6,
	Class = 7,
	Interface = 8,
	Module = 9,
	Property = 10,
	Unit = 11,
	Value = 12,
	Enum = 13,
	Keyword = 14,
	Snippet = 15,
	Color = 16,
	File = 17,
	Reference = 18,
	Folder = 19,
	EnumMember = 20,
	Constant = 21,
	Struct = 22,
	Event = 23,
	Operator = 24,
	TypeParameter = 25
};

 std::vector<std::string> keywords = {
            "if",
            "do",
            "for",
            "let",
            "var",
            "int",
            "try",
            "else",
            "bool",
            "true",
            "case",
            "enum",
            "loop",
            "void",
            "while",
            "break",
            "const",
            "int32",
            "int64",
            "float",
            "false",
            "using",
            "fixed",
            "graph",
            "input",
            "event",
            "class",
            "catch",
            "throw",
            "output",
            "return",
            "string",
            "struct",
            "import",
            "switch",
            "public",
            "double",
            "private",
            "float32",
            "float64",
            "default",
            "complex",
            "continue",
            "external",
            "operator",
            "processor",
            "namespace",
            "complex32",
            "complex64",
            "connection"};


std::map<std::string, ServerFile> fileMap;

nlohmann::json makeLocation(int line, int character)
{
    return {
        {"line", line},
        {"character", character}};
}

nlohmann::json makeRange(nlohmann::json pointA, nlohmann::json pointB)
{
    return {
        {"start", pointA},
        {"end", pointB},
    };
}

void Server::tryCompile(std::string uri)
{
    soul::AST::Allocator allocator;
    soul::pool_ptr<soul::AST::Namespace> topLevelNamespace = soul::AST::createRootNamespace(allocator);
    auto sourceFile = fileMap[uri].file;
    soul::CodeLocation location = soul::CodeLocation::createFromSourceFile(sourceFile);

    soul::CompileMessageList errors;
    soul::CompileMessageHandler handler(errors);
    nlohmann::json response = {
        {"uri", uri},
        {"diagnostics", nlohmann::json::array()}};

    soul::Compiler compiler;
    soul::messageHandler = &handler;
    soul::BuildBundle bundle;
    bundle.settings.sampleRate = 48000;
    bundle.sourceFiles.push_back(sourceFile);

    try
    {
        compiler.build(errors, bundle);
    }
    catch (soul::AbortCompilationException ex)
    {
    }

    for (auto message : errors.messages)
    {
        auto loc = message.location.getLineAndColumn();

        response["diagnostics"]
            .push_back({{"range", makeRange(makeLocation(loc.line - 1, loc.column - 1), makeLocation(loc.line - 1, loc.column))},
                        {"message", message.description}});
    }
    sendNotification("textDocument/publishDiagnostics", response);
}

nlohmann::json Server::initialize(int id, nlohmann::json params)
{
    return {
        {"capabilities", {{"textDocument", {{"publishDiagnostics", true}}}, {"textDocumentSync", 1}, {"completionProvider", {{"resolveProvider", false}}}}}};
}

void Server::open(std::string uri, std::string text)
{
    auto file = soul::SourceFile{uri, text};
    auto entry = ServerFile{
        file,
        text,
        uri};
    fileMap[uri] = entry;
}

void Server::didChange(std::string uri, std::string text)
{
    auto file = soul::SourceFile{uri, text};
    auto entry = ServerFile{
        file,
        text,
        uri};
    fileMap[uri] = entry;
}

void Server::close(std::string uri)
{
    fileMap.erase(uri);
}

nlohmann::json Server::completion() {
        nlohmann::json arr = {};
        for (auto keyword : keywords)
        {
            arr.push_back(
                {
                    {"label", keyword},
                    {"kind", CompletionItemKind::Keyword}
                });
        }
        return arr;
}

void Server::processMessage(int id, std::string method, nlohmann::json params)
{
    nlohmann::json response;
    if (method == "initialize")
    {
        response = initialize(id, params);
    }
    else if (method == "textDocument/completion")
    {
        response = completion();
    }
    sendResponse(id, response);
}

void Server::processNotification(std::string method, nlohmann::json params)
{
    if (method == "textDocument/didOpen")
    {
        open(params["textDocument"]["uri"], params["textDocument"]["text"]);
        tryCompile(params["textDocument"]["uri"]);
    }
    else if (method == "textDocument/didClose")
    {
        close(params["textDocument"]["uri"]);
    }
    else if (method == "textDocument/didChange")
    {
        didChange(params["textDocument"]["uri"], params["contentChanges"][0]["text"]);
        tryCompile(params["textDocument"]["uri"]);
    }
}

void Server::sendNotification(std::string method, nlohmann::json params)
{
    nlohmann::json res = {
        {"jsonrpc", "2.0"},
        {"method", method},
        {"params", params}};
    std::string response = res.dump();
    std::cout << "Content-Length: " << response.size() << "\r\n\r\n"
              << response;
}
void Server::sendResponse(int id, nlohmann::json params)
{
    nlohmann::json res = {
        {"jsonrpc", "2.0"},
        {"id", id},
        {"result", params}};
    std::string response = res.dump();
    std::cout << "Content-Length: " << response.size() << "\r\n\r\n"
              << response;
}