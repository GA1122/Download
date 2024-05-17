void WorkerThread::start(PassOwnPtr<WorkerThreadStartupData> startupData)
{
    if (m_started)
        return;

    m_started = true;
    backingThread().postTask(FROM_HERE, new Task(threadSafeBind(&WorkerThread::initialize, AllowCrossThreadAccess(this), startupData)));
}
