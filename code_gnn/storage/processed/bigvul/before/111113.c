void WebPagePrivate::deferredTasksTimerFired(WebCore::Timer<WebPagePrivate>*)
{
    ASSERT(!m_deferredTasks.isEmpty());
    if (m_deferredTasks.isEmpty())
        return;

    OwnPtr<DeferredTaskBase> task = m_deferredTasks[0].release();
    m_deferredTasks.remove(0);

    if (!m_deferredTasks.isEmpty())
        m_deferredTasksTimer.startOneShot(0);

    task->perform(this);
}
