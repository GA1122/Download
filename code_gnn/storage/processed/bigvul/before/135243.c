void Document::executeScriptsWaitingForResources()
{
    if (!isRenderingReady())
        return;
    if (ScriptableDocumentParser* parser = scriptableDocumentParser())
        parser->executeScriptsWaitingForResources();
}
