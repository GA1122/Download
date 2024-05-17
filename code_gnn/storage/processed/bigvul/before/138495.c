void WorkerThread::appendDebuggerTask(PassOwnPtr<WebThread::Task> task)
{
    m_debuggerMessageQueue.append(task);
}
