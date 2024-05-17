void FrameLoader::didFirstLayout()
{
    if (Page* page = m_frame->page())
        if (isBackForwardLoadType(m_loadType) && page->backForwardList())
            history()->restoreScrollPositionAndViewState();

    if (m_stateMachine.committedFirstRealDocumentLoad() && !m_stateMachine.isDisplayingInitialEmptyDocument() && !m_stateMachine.firstLayoutDone())
        m_stateMachine.advanceTo(FrameLoaderStateMachine::FirstLayoutDone);
    m_client->dispatchDidFirstLayout();
}
