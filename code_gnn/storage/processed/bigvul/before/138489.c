    void OnTimeout()
    {
        ASSERT(m_workerThread->workerGlobalScope());

        m_lastQueuedTask = nullptr;

        if (m_sharedTimerFunction && m_running && !m_workerThread->workerGlobalScope()->isClosing())
            m_sharedTimerFunction();
    }
