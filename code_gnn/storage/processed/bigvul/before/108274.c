void FrameLoader::provisionalLoadStarted()
{
    if (m_stateMachine.firstLayoutDone())
        m_stateMachine.advanceTo(FrameLoaderStateMachine::CommittedFirstRealLoad);
    m_frame->redirectScheduler()->cancel(true);
    m_client->provisionalLoadStarted();
}
