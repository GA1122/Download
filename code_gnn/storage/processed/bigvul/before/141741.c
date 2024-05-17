V8Debugger::PauseOnExceptionsState V8Debugger::getPauseOnExceptionsState()
{
    DCHECK(enabled());
    v8::HandleScope scope(m_isolate);
    v8::Context::Scope contextScope(debuggerContext());

    v8::Local<v8::Value> argv[] = { v8::Undefined(m_isolate) };
    v8::Local<v8::Value> result = callDebuggerMethod("pauseOnExceptionsState", 0, argv).ToLocalChecked();
    return static_cast<V8Debugger::PauseOnExceptionsState>(result->Int32Value());
}
