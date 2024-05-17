void WorkerThread::interruptAndDispatchInspectorCommands()
{
    MutexLocker locker(m_workerInspectorControllerMutex);
    if (m_workerInspectorController)
        m_workerInspectorController->interruptAndDispatchInspectorCommands();
}
