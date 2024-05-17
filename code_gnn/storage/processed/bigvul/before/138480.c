void HTMLScriptRunner::executeScriptsWaitingForResources()
{
    ASSERT(m_document);
    ASSERT(hasScriptsWaitingForResources());
    ASSERT(!isExecutingScript());
    ASSERT(m_document->isScriptExecutionReady());
    executeParsingBlockingScripts();
}
