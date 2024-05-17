PlatformThreadId WorkerThread::platformThreadId()
{
    if (!m_started)
        return 0;
    return backingThread().platformThread().threadId();
}
