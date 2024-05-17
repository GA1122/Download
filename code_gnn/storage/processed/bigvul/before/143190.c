void Document::executeScriptsWaitingForResources()
{
    if (!isScriptExecutionReady())
        return;
    if (ScriptableDocumentParser* parser = scriptableDocumentParser())
        parser->executeScriptsWaitingForResources();
}
