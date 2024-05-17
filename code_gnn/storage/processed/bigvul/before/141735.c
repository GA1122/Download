v8::Local<v8::Context> V8Debugger::debuggerContext() const
{
    DCHECK(!m_debuggerContext.IsEmpty());
     return m_debuggerContext.Get(m_isolate);
 }
