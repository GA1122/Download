void V8Debugger::setPauseOnNextStatement(bool pause)
{
    if (m_runningNestedMessageLoop)
        return;
    if (pause)
        v8::Debug::DebugBreak(m_isolate);
    else
        v8::Debug::CancelDebugBreak(m_isolate);
}
