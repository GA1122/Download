void DocumentThreadableLoader::clear()
{
    m_client = nullptr;

    if (!m_async)
        return;

    m_timeoutTimer.stop();
    m_requestStartedSeconds = 0.0;
    clearResource();
}
