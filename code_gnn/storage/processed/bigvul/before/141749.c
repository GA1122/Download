String16 V8Debugger::setBreakpoint(const String16& sourceID, const ScriptBreakpoint& scriptBreakpoint, int* actualLineNumber, int* actualColumnNumber)
{
    v8::HandleScope scope(m_isolate);
    v8::Context::Scope contextScope(debuggerContext());

    v8::Local<v8::Object> info = v8::Object::New(m_isolate);
    info->Set(toV8StringInternalized(m_isolate, "sourceID"), toV8String(m_isolate, sourceID));
    info->Set(toV8StringInternalized(m_isolate, "lineNumber"), v8::Integer::New(m_isolate, scriptBreakpoint.lineNumber));
    info->Set(toV8StringInternalized(m_isolate, "columnNumber"), v8::Integer::New(m_isolate, scriptBreakpoint.columnNumber));
    info->Set(toV8StringInternalized(m_isolate, "condition"), toV8String(m_isolate, scriptBreakpoint.condition));

    v8::Local<v8::Function> setBreakpointFunction = v8::Local<v8::Function>::Cast(m_debuggerScript.Get(m_isolate)->Get(toV8StringInternalized(m_isolate, "setBreakpoint")));
    v8::Local<v8::Value> breakpointId = v8::Debug::Call(debuggerContext(), setBreakpointFunction, info).ToLocalChecked();
    if (!breakpointId->IsString())
        return "";
    *actualLineNumber = info->Get(toV8StringInternalized(m_isolate, "lineNumber"))->Int32Value();
    *actualColumnNumber = info->Get(toV8StringInternalized(m_isolate, "columnNumber"))->Int32Value();
    return toProtocolString(breakpointId.As<v8::String>());
}
