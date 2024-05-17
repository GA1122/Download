void HTMLScriptRunner::detach()
{
    if (!m_document)
        return;

    m_parserBlockingScript.stopWatchingForLoad(this);
    m_parserBlockingScript.releaseElementAndClear();

    while (!m_scriptsToExecuteAfterParsing.isEmpty()) {
        PendingScript pendingScript = m_scriptsToExecuteAfterParsing.takeFirst();
        pendingScript.stopWatchingForLoad(this);
        pendingScript.releaseElementAndClear();
    }
    m_document = nullptr;
}
