void V8Debugger::compileDebuggerScript()
{
    if (!m_debuggerScript.IsEmpty()) {
        NOTREACHED();
        return;
    }

    v8::HandleScope scope(m_isolate);
    v8::Context::Scope contextScope(debuggerContext());

    v8::Local<v8::String> scriptValue = v8::String::NewFromUtf8(m_isolate, DebuggerScript_js, v8::NewStringType::kInternalized, sizeof(DebuggerScript_js)).ToLocalChecked();
    v8::Local<v8::Value> value;
    if (!m_inspector->compileAndRunInternalScript(debuggerContext(), scriptValue).ToLocal(&value)) {
        NOTREACHED();
        return;
    }
    DCHECK(value->IsObject());
    m_debuggerScript.Reset(m_isolate, value.As<v8::Object>());
}
