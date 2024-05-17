void FrameLoader::didExplicitOpen()
{
    m_isComplete = false;
    m_didCallImplicitClose = false;

    if (!m_stateMachine.committedFirstRealDocumentLoad())
        m_stateMachine.advanceTo(FrameLoaderStateMachine::DisplayingInitialEmptyDocumentPostCommit);
    
    m_frame->redirectScheduler()->cancel(); 
    if (m_frame->document()->url() != blankURL())
        m_URL = m_frame->document()->url();
}
