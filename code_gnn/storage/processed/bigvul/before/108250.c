void FrameLoader::dispatchDidCommitLoad()
{
    if (m_stateMachine.creatingInitialEmptyDocument())
        return;

#ifndef NDEBUG
    m_didDispatchDidCommitLoad = true;
#endif

    m_client->dispatchDidCommitLoad();

#if ENABLE(INSPECTOR)
    if (Page* page = m_frame->page())
        page->inspectorController()->didCommitLoad(m_documentLoader.get());
#endif
}
