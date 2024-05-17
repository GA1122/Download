void V8Debugger::clearStepping()
{
    DCHECK(enabled());
    v8::HandleScope scope(m_isolate);
    v8::Context::Scope contextScope(debuggerContext());

    v8::Local<v8::Value> argv[] = { v8::Undefined(m_isolate) };
    callDebuggerMethod("clearStepping", 0, argv);
}
