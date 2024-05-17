    virtual void stop()
    {
        m_running = false;
        m_lastQueuedTask = nullptr;
    }
