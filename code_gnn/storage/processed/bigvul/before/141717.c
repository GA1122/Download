void V8Debugger::allAsyncTasksCanceled()
{
    m_asyncTaskStacks.clear();
    m_recurringTasks.clear();
    m_currentStacks.clear();
    m_currentTasks.clear();
}
