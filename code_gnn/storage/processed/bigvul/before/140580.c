bool V8DOMWrapper::isWrapper(v8::Isolate* isolate, v8::Local<v8::Value> value)
{
    if (value.IsEmpty() || !value->IsObject())
        return false;
    v8::Local<v8::Object> object = v8::Local<v8::Object>::Cast(value);

    if (object->InternalFieldCount() < v8DefaultWrapperInternalFieldCount)
        return false;

    const WrapperTypeInfo* untrustedWrapperTypeInfo = toWrapperTypeInfo(object);
    V8PerIsolateData* perIsolateData = V8PerIsolateData::from(isolate);
    if (!(untrustedWrapperTypeInfo && perIsolateData))
        return false;
    return perIsolateData->hasInstance(untrustedWrapperTypeInfo, object);
}
