void V8Debugger::getCompiledScripts(int contextGroupId, std::vector<std::unique_ptr<V8DebuggerScript>>& result)
{
    v8::HandleScope scope(m_isolate);
    v8::MicrotasksScope microtasks(m_isolate, v8::MicrotasksScope::kDoNotRunMicrotasks);
    v8::Local<v8::Object> debuggerScript = m_debuggerScript.Get(m_isolate);
    DCHECK(!debuggerScript->IsUndefined());
    v8::Local<v8::Function> getScriptsFunction = v8::Local<v8::Function>::Cast(debuggerScript->Get(toV8StringInternalized(m_isolate, "getScripts")));
    v8::Local<v8::Value> argv[] = { v8::Integer::New(m_isolate, contextGroupId) };
    v8::Local<v8::Value> value;
    if (!getScriptsFunction->Call(debuggerContext(), debuggerScript, PROTOCOL_ARRAY_LENGTH(argv), argv).ToLocal(&value))
        return;
    DCHECK(value->IsArray());
    v8::Local<v8::Array> scriptsArray = v8::Local<v8::Array>::Cast(value);
    result.reserve(scriptsArray->Length());
    for (unsigned i = 0; i < scriptsArray->Length(); ++i) {
        v8::Local<v8::Object> scriptObject = v8::Local<v8::Object>::Cast(scriptsArray->Get(v8::Integer::New(m_isolate, i)));
        result.push_back(wrapUnique(new V8DebuggerScript(m_isolate, scriptObject, inLiveEditScope)));
    }
}
