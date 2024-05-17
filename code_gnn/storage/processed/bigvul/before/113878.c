bool HTMLDocumentParser::canTakeNextToken(SynchronousMode mode, PumpSession& session)
{
    if (isStopped())
        return false;

    if (isWaitingForScripts()) {
        if (mode == AllowYield)
            m_parserScheduler->checkForYieldBeforeScript(session);

        if (session.needsYield)
            return false;

        runScriptsForPausedTreeBuilder();
        if (isWaitingForScripts() || isStopped())
            return false;
    }

    if (!isParsingFragment()
        && document()->frame() && document()->frame()->navigationScheduler()->locationChangePending())
        return false;

    if (mode == AllowYield)
        m_parserScheduler->checkForYieldBeforeToken(session);

    return true;
}
