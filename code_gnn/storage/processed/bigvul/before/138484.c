void HTMLScriptRunner::requestDeferredScript(Element* element)
{
    PendingScript pendingScript;
    if (!requestPendingScript(pendingScript, element))
        return;

    if (m_document->frame() && !pendingScript.isReady())
        ScriptStreamer::startStreaming(pendingScript, PendingScript::Deferred, m_document->frame()->settings(), ScriptState::forMainWorld(m_document->frame()));

    ASSERT(pendingScript.resource());
    m_scriptsToExecuteAfterParsing.append(pendingScript);
}
