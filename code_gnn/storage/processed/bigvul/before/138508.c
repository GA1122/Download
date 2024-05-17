bool WorkerThread::isCurrentThread()
{
    return m_started && backingThread().isCurrentThread();
}
