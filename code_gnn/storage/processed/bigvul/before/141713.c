void V8Console::valuesCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    v8::Isolate* isolate = info.GetIsolate();
    info.GetReturnValue().Set(v8::Array::New(isolate));

    ConsoleHelper helper(info);
    v8::Local<v8::Object> obj;
    if (!helper.firstArgAsObject().ToLocal(&obj))
        return;
    v8::Local<v8::Array> names;
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    if (!obj->GetOwnPropertyNames(context).ToLocal(&names))
        return;
    v8::Local<v8::Array> values = v8::Array::New(isolate, names->Length());
    for (size_t i = 0; i < names->Length(); ++i) {
        v8::Local<v8::Value> key;
        if (!names->Get(context, i).ToLocal(&key))
            continue;
        v8::Local<v8::Value> value;
        if (!obj->Get(context, key).ToLocal(&value))
            continue;
        if (!values->Set(context, i, value).FromMaybe(false))
            continue;
    }
    info.GetReturnValue().Set(values);
}
