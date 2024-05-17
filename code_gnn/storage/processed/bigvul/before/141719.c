void V8Debugger::asyncTaskFinished(void* task)
{
    if (!m_maxAsyncCallStackDepth)
        return;
    if (!m_currentStacks.size())
        return;

    DCHECK(m_currentTasks.back() == task);
    m_currentTasks.pop_back();

    m_currentStacks.pop_back();
    if (m_recurringTasks.find(task) == m_recurringTasks.end())
        m_asyncTaskStacks.erase(task);
}
