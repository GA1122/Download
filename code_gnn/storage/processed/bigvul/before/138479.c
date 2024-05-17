bool HTMLScriptRunner::executeScriptsWaitingForParsing()
{
    while (!m_scriptsToExecuteAfterParsing.isEmpty()) {
        ASSERT(!isExecutingScript());
        ASSERT(!hasParserBlockingScript());
        ASSERT(m_scriptsToExecuteAfterParsing.first().resource());
        if (!m_scriptsToExecuteAfterParsing.first().isReady()) {
            m_scriptsToExecuteAfterParsing.first().watchForLoad(this);
            return false;
        }
        PendingScript first = m_scriptsToExecuteAfterParsing.takeFirst();
        executePendingScriptAndDispatchEvent(first, PendingScript::Deferred);
        if (!m_document)
            return false;
    }
    return true;
}
