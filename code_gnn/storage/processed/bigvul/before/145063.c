    void runAllTasks()
    {
        while (!m_tasks.isEmpty())
            m_tasks.takeFirst()->run();
    }
