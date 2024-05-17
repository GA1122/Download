void Document::resumeScheduledTasks()
{
    ExecutionContext::resumeScheduledTasks();
    m_taskRunner->resume();
}
