void HTMLScriptRunner::requestParsingBlockingScript(Element* element)
{
    if (!requestPendingScript(m_parserBlockingScript, element))
        return;

    ASSERT(m_parserBlockingScript.resource());

    if (!m_parserBlockingScript.isReady()) {
        if (m_document->frame())
            ScriptStreamer::startStreaming(m_parserBlockingScript, PendingScript::ParsingBlocking, m_document->frame()->settings(), ScriptState::forMainWorld(m_document->frame()));

        m_parserBlockingScript.watchForLoad(this);
    }
}
