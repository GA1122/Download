void V8Debugger::setBreakpointsActivated(bool activated)
{
    if (!enabled()) {
        NOTREACHED();
        return;
    }
    v8::HandleScope scope(m_isolate);
    v8::Context::Scope contextScope(debuggerContext());

    v8::Local<v8::Object> info = v8::Object::New(m_isolate);
    info->Set(toV8StringInternalized(m_isolate, "enabled"), v8::Boolean::New(m_isolate, activated));
    v8::Local<v8::Function> setBreakpointsActivated = v8::Local<v8::Function>::Cast(m_debuggerScript.Get(m_isolate)->Get(toV8StringInternalized(m_isolate, "setBreakpointsActivated")));
    v8::Debug::Call(debuggerContext(), setBreakpointsActivated, info).ToLocalChecked();

    m_breakpointsActivated = activated;
}
