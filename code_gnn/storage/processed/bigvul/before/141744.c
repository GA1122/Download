bool V8Debugger::isPaused()
{
    return !m_pausedContext.IsEmpty();
}
