void Document::tasksWereSuspended()
{
    scriptRunner()->suspend();

    if (m_parser)
        m_parser->suspendScheduledTasks();
    if (m_scriptedAnimationController)
        m_scriptedAnimationController->suspend();
}
