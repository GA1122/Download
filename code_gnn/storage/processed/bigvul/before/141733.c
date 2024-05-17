V8StackTraceImpl* V8Debugger::currentAsyncCallChain()
{
    if (!m_currentStacks.size())
        return nullptr;
    return m_currentStacks.back().get();
}
