v8::MaybeLocal<v8::Value> V8Debugger::callDebuggerMethod(const char* functionName, int argc, v8::Local<v8::Value> argv[])
{
    v8::MicrotasksScope microtasks(m_isolate, v8::MicrotasksScope::kDoNotRunMicrotasks);
    v8::Local<v8::Object> debuggerScript = m_debuggerScript.Get(m_isolate);
    v8::Local<v8::Function> function = v8::Local<v8::Function>::Cast(debuggerScript->Get(toV8StringInternalized(m_isolate, functionName)));
    DCHECK(m_isolate->InContext());
    return function->Call(m_isolate->GetCurrentContext(), debuggerScript, argc, argv);
}
