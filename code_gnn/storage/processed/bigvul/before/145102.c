        void enterRunLoop()
        {
            if (m_taskObserver)
                m_taskObserver->willProcessTask();
            if (m_taskRunner.m_task) {
                m_taskRunner.m_task->run();
                delete m_taskRunner.m_task;
                m_taskRunner.m_task = 0;
            }
            if (m_taskObserver)
                m_taskObserver->didProcessTask();
        }
