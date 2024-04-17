const vscode = require('vscode');

function activate(context) {
    console.log('Congratulations, your extension "cpy-language" is now active!');

    // Register the language configuration
    vscode.languages.setLanguageConfiguration('cpy', {
        wordPattern: /("(?:[^\\\"]*(?:\\.)?)*")|([^\s]+)/g
    });

    // Register the language provider
    context.subscriptions.push(vscode.languages.registerCompletionItemProvider('cpy', {
        provideCompletionItems(document, position, token, context) {
            // TODO: Implement completion items
            return [
                new vscode.CompletionItem('main', vscode.CompletionItemKind.Function),
                new vscode.CompletionItem('print', vscode.CompletionItemKind.Function),
                new vscode.CompletionItem('int', vscode.CompletionItemKind.Keyword)
            ];
        }
    }));
}

function deactivate() {}

module.exports = {
    activate,
    deactivate
};
