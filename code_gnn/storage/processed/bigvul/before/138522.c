void WorkerThread::stopInternal()
{
    MutexLocker lock(m_threadStateMutex);

    if (m_terminated)
        return;
    m_terminated = true;

    if (m_shutdownEvent)
        m_shutdownEvent->signal();

    if (m_shutdown)
        return;

    if (!m_workerGlobalScope) {
        m_terminationEvent->signal();
        return;
    }

    terminateV8Execution();

    InspectorInstrumentation::didKillAllExecutionContextTasks(m_workerGlobalScope.get());
    m_debuggerMessageQueue.kill();
    backingThread().postTask(FROM_HERE, new Task(threadSafeBind(&WorkerThread::shutdown, AllowCrossThreadAccess(this))));
}
