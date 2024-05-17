void V8Debugger::setPauseOnExceptionsState(PauseOnExceptionsState pauseOnExceptionsState)
{
    DCHECK(enabled());
    v8::HandleScope scope(m_isolate);
    v8::Context::Scope contextScope(debuggerContext());

    v8::Local<v8::Value> argv[] = { v8::Int32::New(m_isolate, pauseOnExceptionsState) };
    callDebuggerMethod("setPauseOnExceptionsState", 1, argv);
}
