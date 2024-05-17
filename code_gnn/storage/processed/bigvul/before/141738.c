bool V8Debugger::enabled() const
{
    return !m_debuggerScript.IsEmpty();
}
