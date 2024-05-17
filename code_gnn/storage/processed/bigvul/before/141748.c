void V8Debugger::setAsyncCallStackDepth(V8DebuggerAgentImpl* agent, int depth)
{
    if (depth <= 0)
        m_maxAsyncCallStackDepthMap.erase(agent);
    else
        m_maxAsyncCallStackDepthMap[agent] = depth;

    int maxAsyncCallStackDepth = 0;
    for (const auto& pair : m_maxAsyncCallStackDepthMap) {
        if (pair.second > maxAsyncCallStackDepth)
            maxAsyncCallStackDepth = pair.second;
    }

    if (m_maxAsyncCallStackDepth == maxAsyncCallStackDepth)
        return;
    m_maxAsyncCallStackDepth = maxAsyncCallStackDepth;
    if (!maxAsyncCallStackDepth)
        allAsyncTasksCanceled();
}
