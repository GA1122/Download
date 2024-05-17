void V8Debugger::breakProgram()
{
    if (isPaused()) {
        DCHECK(!m_runningNestedMessageLoop);
        v8::Local<v8::Value> exception;
        v8::Local<v8::Array> hitBreakpoints;
        handleProgramBreak(m_pausedContext, m_executionState, exception, hitBreakpoints);
        return;
    }

    if (!canBreakProgram())
        return;

    v8::HandleScope scope(m_isolate);
    v8::Local<v8::Function> breakFunction;
    if (!V8_FUNCTION_NEW_REMOVE_PROTOTYPE(m_isolate->GetCurrentContext(), &V8Debugger::breakProgramCallback, v8::External::New(m_isolate, this), 0).ToLocal(&breakFunction))
        return;
    v8::Debug::Call(debuggerContext(), breakFunction).ToLocalChecked();
}
