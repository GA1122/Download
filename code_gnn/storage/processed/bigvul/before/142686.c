void FrameLoader::finishedParsing()
{
    if (m_stateMachine.creatingInitialEmptyDocument())
        return;

    m_progressTracker->finishedParsing();

    if (client()) {
        ScriptForbiddenScope forbidScripts;
        client()->dispatchDidFinishDocumentLoad();
    }

    if (client())
        client()->runScriptsAtDocumentReady(m_documentLoader ? m_documentLoader->isCommittedButEmpty() : true);

    checkCompleted();

    if (!m_frame->view())
        return;  

    m_frame->view()->restoreScrollbar();
    processFragment(m_frame->document()->url(), NavigationToDifferentDocument);
}
