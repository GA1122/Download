JavaScriptCallFrames V8Debugger::currentCallFrames(int limit)
{
    if (!m_isolate->InContext())
        return JavaScriptCallFrames();
    v8::Local<v8::Value> currentCallFramesV8;
    if (m_executionState.IsEmpty()) {
        v8::Local<v8::Function> currentCallFramesFunction = v8::Local<v8::Function>::Cast(m_debuggerScript.Get(m_isolate)->Get(toV8StringInternalized(m_isolate, "currentCallFrames")));
        currentCallFramesV8 = v8::Debug::Call(debuggerContext(), currentCallFramesFunction, v8::Integer::New(m_isolate, limit)).ToLocalChecked();
    } else {
        v8::Local<v8::Value> argv[] = { m_executionState, v8::Integer::New(m_isolate, limit) };
        currentCallFramesV8 = callDebuggerMethod("currentCallFrames", PROTOCOL_ARRAY_LENGTH(argv), argv).ToLocalChecked();
    }
    DCHECK(!currentCallFramesV8.IsEmpty());
    if (!currentCallFramesV8->IsArray())
        return JavaScriptCallFrames();
    v8::Local<v8::Array> callFramesArray = currentCallFramesV8.As<v8::Array>();
    JavaScriptCallFrames callFrames;
    for (size_t i = 0; i < callFramesArray->Length(); ++i) {
        v8::Local<v8::Value> callFrameValue;
        if (!callFramesArray->Get(debuggerContext(), i).ToLocal(&callFrameValue))
            return JavaScriptCallFrames();
        if (!callFrameValue->IsObject())
            return JavaScriptCallFrames();
        v8::Local<v8::Object> callFrameObject = callFrameValue.As<v8::Object>();
        callFrames.push_back(JavaScriptCallFrame::create(debuggerContext(), v8::Local<v8::Object>::Cast(callFrameObject)));
    }
    return callFrames;
}
