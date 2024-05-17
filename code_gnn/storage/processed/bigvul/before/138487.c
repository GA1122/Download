void HTMLScriptRunner::stopWatchingResourceForLoad(Resource* resource)
{
    if (m_parserBlockingScript.resource() == resource) {
        m_parserBlockingScript.stopWatchingForLoad(this);
        m_parserBlockingScript.releaseElementAndClear();
        return;
    }
    for (PendingScript& script : m_scriptsToExecuteAfterParsing) {
        if (script.resource() == resource) {
            script.stopWatchingForLoad(this);
            script.releaseElementAndClear();
            return;
        }
    }
}
