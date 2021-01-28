#include "Server.hpp"
#include "SOUL/source/modules/soul_core/soul_core.cpp"

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

void Server::tryCompile(std::string path, std::string file)
{
    soul::AST::Allocator allocator;
    soul::pool_ptr<soul::AST::Namespace> topLevelNamespace = soul::AST::createRootNamespace(allocator);
    auto sourceFile = soul::SourceFile{path, file};
    soul::CodeLocation location = soul::CodeLocation::createFromSourceFile(sourceFile);

    soul::CompileMessageList errors;
    soul::CompileMessageHandler handler(errors);
    nlohmann::json response = {
        {"uri", path},
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
        {"capabilities", {{"textDocument", {{"publishDiagnostics", true}}}, {"textDocumentSync", 1}}}};
}

void Server::processMessage(int id, std::string method, nlohmann::json params)
{
    nlohmann::json response;
    if (method == "initialize")
    {
        response = initialize(id, params);
    }
    sendResponse(id, response);
}

void Server::processNotification(std::string method, nlohmann::json params)
{
    if (method == "textDocument/didOpen")
    {
        tryCompile(params["textDocument"]["uri"], params["textDocument"]["text"]);
    }
    else if (method == "textDocument/didChange")
    {
        tryCompile(params["textDocument"]["uri"], params["contentChanges"][0]["text"]);
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