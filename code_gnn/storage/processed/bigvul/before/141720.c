void V8Debugger::asyncTaskStarted(void* task)
{
    if (!m_maxAsyncCallStackDepth)
        return;
    m_currentTasks.push_back(task);
    AsyncTaskToStackTrace::iterator stackIt = m_asyncTaskStacks.find(task);
    std::unique_ptr<V8StackTraceImpl> stack;
    if (stackIt != m_asyncTaskStacks.end() && stackIt->second)
        stack = stackIt->second->cloneImpl();
    m_currentStacks.push_back(std::move(stack));
}
