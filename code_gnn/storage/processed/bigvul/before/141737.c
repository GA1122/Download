void V8Debugger::enable()
{
    if (m_enableCount++)
        return;
    DCHECK(!enabled());
    v8::HandleScope scope(m_isolate);
    v8::Debug::SetDebugEventListener(m_isolate, &V8Debugger::v8DebugEventCallback, v8::External::New(m_isolate, this));
    m_debuggerContext.Reset(m_isolate, v8::Debug::GetDebugContext(m_isolate));
    compileDebuggerScript();
}
