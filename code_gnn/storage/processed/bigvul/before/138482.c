bool HTMLScriptRunner::isPendingScriptReady(const PendingScript& script)
{
    m_hasScriptsWaitingForResources = !m_document->isScriptExecutionReady();
    if (m_hasScriptsWaitingForResources)
        return false;
    return script.isReady();
}
