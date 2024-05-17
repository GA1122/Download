void V8Debugger::removeBreakpoint(const String16& breakpointId)
{
    v8::HandleScope scope(m_isolate);
    v8::Context::Scope contextScope(debuggerContext());

    v8::Local<v8::Object> info = v8::Object::New(m_isolate);
    info->Set(toV8StringInternalized(m_isolate, "breakpointId"), toV8String(m_isolate, breakpointId));

    v8::Local<v8::Function> removeBreakpointFunction = v8::Local<v8::Function>::Cast(m_debuggerScript.Get(m_isolate)->Get(toV8StringInternalized(m_isolate, "removeBreakpoint")));
    v8::Debug::Call(debuggerContext(), removeBreakpointFunction, info).ToLocalChecked();
}
