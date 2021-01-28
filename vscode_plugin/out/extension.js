"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.deactivate = exports.activate = void 0;
const vscode_1 = require("vscode");
const node_1 = require("vscode-languageclient/node");
let client;
function activate(context) {
    let serverExecutable = "../build/parser"; // TODO: move this to configuration file
    let debugOptions = { execArgv: [] };
    let serverOptions = {
        run: { command: serverExecutable, args: [], options: {} },
        debug: { command: serverExecutable, args: [], options: {} },
    };
    let clientOptions = {
        documentSelector: [{ scheme: "file", language: "soul" }],
        synchronize: {
            fileEvents: vscode_1.workspace.createFileSystemWatcher("**/.clientrc"),
        },
    };
    // Create the language client and start the client.
    client = new node_1.LanguageClient("soulLanguageServer", "SOUL Language Server", serverOptions, clientOptions);
    // Start the client. This will also launch the server
    client.start();
}
exports.activate = activate;
function deactivate() {
    if (!client) {
        return undefined;
    }
    return client.stop();
}
exports.deactivate = deactivate;
//# sourceMappingURL=extension.js.map