void V8InjectedScriptHost::subtypeCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.Length() < 1)
        return;

    v8::Isolate* isolate = info.GetIsolate();
    v8::Local<v8::Value> value = info[0];
    if (value->IsObject()) {
        v8::Local<v8::Value> internalType = v8InternalValueTypeFrom(isolate->GetCurrentContext(), v8::Local<v8::Object>::Cast(value));
        if (internalType->IsString()) {
            info.GetReturnValue().Set(internalType);
            return;
        }
    }
    if (value->IsArray() || value->IsArgumentsObject()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "array"));
        return;
    }
    if (value->IsTypedArray()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "typedarray"));
        return;
    }
    if (value->IsDate()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "date"));
        return;
    }
    if (value->IsRegExp()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "regexp"));
        return;
    }
    if (value->IsMap() || value->IsWeakMap()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "map"));
        return;
    }
    if (value->IsSet() || value->IsWeakSet()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "set"));
        return;
    }
    if (value->IsMapIterator() || value->IsSetIterator()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "iterator"));
        return;
    }
    if (value->IsGeneratorObject()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "generator"));
        return;
    }
    if (value->IsNativeError()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "error"));
        return;
    }
    if (value->IsProxy()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "proxy"));
        return;
    }
    if (value->IsPromise()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "promise"));
        return;
    }
    String16 subtype = unwrapInspector(info)->client()->valueSubtype(value);
    if (!subtype.isEmpty()) {
        info.GetReturnValue().Set(toV8String(isolate, subtype));
        return;
    }
}
