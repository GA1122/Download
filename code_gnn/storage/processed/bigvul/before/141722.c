void V8Debugger::breakProgramCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    DCHECK_EQ(info.Length(), 2);
    V8Debugger* thisPtr = toV8Debugger(info.Data());
    if (!thisPtr->enabled())
        return;
    v8::Local<v8::Context> pausedContext = thisPtr->m_isolate->GetCurrentContext();
    v8::Local<v8::Value> exception;
    v8::Local<v8::Array> hitBreakpoints;
    thisPtr->handleProgramBreak(pausedContext, v8::Local<v8::Object>::Cast(info[0]), exception, hitBreakpoints);
}
