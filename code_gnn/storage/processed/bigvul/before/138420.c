void Document::executeScriptsWaitingForResourcesTimerFired(Timer<Document>*)
{
    if (!isRenderingReady())
        return;
    if (ScriptableDocumentParser* parser = scriptableDocumentParser())
        parser->executeScriptsWaitingForResources();
}
