void V8Debugger::continueProgram()
{
    if (isPaused())
        m_inspector->client()->quitMessageLoopOnPause();
    m_pausedContext.Clear();
    m_executionState.Clear();
}
