void V8Debugger::clearBreakpoints()
{
    v8::HandleScope scope(m_isolate);
    v8::Context::Scope contextScope(debuggerContext());

    v8::Local<v8::Function> clearBreakpoints = v8::Local<v8::Function>::Cast(m_debuggerScript.Get(m_isolate)->Get(toV8StringInternalized(m_isolate, "clearBreakpoints")));
    v8::Debug::Call(debuggerContext(), clearBreakpoints).ToLocalChecked();
}
