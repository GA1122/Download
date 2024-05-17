void Document::suspendScheduledTasks()
{
    ASSERT(!m_scheduledTasksAreSuspended);

    suspendScriptedAnimationControllerCallbacks();
    suspendActiveDOMObjects();
    scriptRunner()->suspend();
    m_pendingTasksTimer.stop();
    if (m_parser)
        m_parser->suspendScheduledTasks();

    m_scheduledTasksAreSuspended = true;
}
