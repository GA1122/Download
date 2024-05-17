void HTMLDocumentParser::prepareToStopParsing()
{
    ASSERT(!hasInsertionPoint());

    RefPtr<HTMLDocumentParser> protect(this);

    pumpTokenizerIfPossible(ForceSynchronous);
    
    if (isStopped())
        return;

    DocumentParser::prepareToStopParsing();

    if (m_scriptRunner)
        document()->setReadyState(Document::Interactive);

    if (isDetached())
        return;

    attemptToRunDeferredScriptsAndEnd();
}
