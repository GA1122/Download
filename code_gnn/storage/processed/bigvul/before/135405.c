void Document::suspendScheduledTasks()
{
    ExecutionContext::suspendScheduledTasks();
    m_taskRunner->suspend();
}
