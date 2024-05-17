v8::Local<v8::Value> V8Debugger::callInternalGetterFunction(v8::Local<v8::Object> object, const char* functionName)
{
    v8::MicrotasksScope microtasks(m_isolate, v8::MicrotasksScope::kDoNotRunMicrotasks);
    v8::Local<v8::Value> getterValue = object->Get(toV8StringInternalized(m_isolate, functionName));
    DCHECK(!getterValue.IsEmpty() && getterValue->IsFunction());
    return v8::Local<v8::Function>::Cast(getterValue)->Call(m_isolate->GetCurrentContext(), object, 0, 0).ToLocalChecked();
}
