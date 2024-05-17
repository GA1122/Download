void Document::pendingTasksTimerFired(Timer<Document>*)
{
    while (!m_pendingTasks.isEmpty()) {
        OwnPtr<Task> task = m_pendingTasks[0].release();
        m_pendingTasks.remove(0);
        task->performTask(this);
    }
}
