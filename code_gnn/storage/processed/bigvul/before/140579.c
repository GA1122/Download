bool V8DOMWrapper::hasInternalFieldsSet(v8::Local<v8::Value> value)
{
    if (value.IsEmpty() || !value->IsObject())
        return false;
    v8::Local<v8::Object> object = v8::Local<v8::Object>::Cast(value);

    if (object->InternalFieldCount() < v8DefaultWrapperInternalFieldCount)
        return false;

    const ScriptWrappable* untrustedScriptWrappable = toScriptWrappable(object);
    const WrapperTypeInfo* untrustedWrapperTypeInfo = toWrapperTypeInfo(object);
    return untrustedScriptWrappable
        && untrustedWrapperTypeInfo
        && untrustedWrapperTypeInfo->ginEmbedder == gin::kEmbedderBlink;
}
