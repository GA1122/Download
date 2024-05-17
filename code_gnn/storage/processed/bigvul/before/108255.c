void FrameLoader::frameLoadCompleted()
{

    m_client->frameLoadCompleted();

    history()->updateForFrameLoadCompleted();

    if (m_documentLoader && m_stateMachine.committedFirstRealDocumentLoad() && !m_stateMachine.isDisplayingInitialEmptyDocument() && !m_stateMachine.firstLayoutDone())
        m_stateMachine.advanceTo(FrameLoaderStateMachine::FirstLayoutDone);
}
