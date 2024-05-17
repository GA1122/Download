std::unique_ptr<V8StackTraceImpl> V8Debugger::captureStackTrace(bool fullStack)
{
    if (!m_isolate->InContext())
        return nullptr;

    v8::HandleScope handles(m_isolate);
    int contextGroupId = getGroupId(m_isolate->GetCurrentContext());
    if (!contextGroupId)
        return nullptr;

    size_t stackSize = fullStack ? V8StackTraceImpl::maxCallStackSizeToCapture : 1;
    if (m_inspector->enabledRuntimeAgentForGroup(contextGroupId))
        stackSize = V8StackTraceImpl::maxCallStackSizeToCapture;

    return V8StackTraceImpl::capture(this, contextGroupId, stackSize);
}
