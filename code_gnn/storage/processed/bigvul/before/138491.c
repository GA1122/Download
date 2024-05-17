    explicit WorkerSharedTimer(WorkerThread* workerThread)
        : m_workerThread(workerThread)
        , m_running(false)
    { }
