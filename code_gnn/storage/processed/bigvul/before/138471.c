HTMLScriptRunner::HTMLScriptRunner(Document* document, HTMLScriptRunnerHost* host)
    : m_document(document)
    , m_host(host)
    , m_scriptNestingLevel(0)
    , m_hasScriptsWaitingForResources(false)
    , m_parserBlockingScriptAlreadyLoaded(false)
{
    ASSERT(m_host);
}
