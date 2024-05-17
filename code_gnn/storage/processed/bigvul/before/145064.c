    void runSingleTask()
    {
        if (m_tasks.isEmpty())
            return;
        m_tasks.takeFirst()->run();
    }
