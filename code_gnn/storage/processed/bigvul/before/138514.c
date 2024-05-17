    virtual void setFireInterval(double interval)
    {
        ASSERT(m_sharedTimerFunction);

        int64_t delay = static_cast<int64_t>(ceil(interval * 1000));

        if (delay < 0) {
            delay = 0;
        }

        m_running = true;

        if (m_lastQueuedTask.get())
            m_lastQueuedTask->cancelTask();

        OwnPtr<WorkerThreadCancelableTask> task = WorkerThreadCancelableTask::create(bind(&WorkerSharedTimer::OnTimeout, this));
        m_lastQueuedTask = task->createWeakPtr();
        m_workerThread->postDelayedTask(FROM_HERE, task.release(), delay);
    }
