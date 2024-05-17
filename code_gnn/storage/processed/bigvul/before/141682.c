void V8Console::keysCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    v8::Isolate* isolate = info.GetIsolate();
    info.GetReturnValue().Set(v8::Array::New(isolate));

    ConsoleHelper helper(info);
    v8::Local<v8::Object> obj;
    if (!helper.firstArgAsObject().ToLocal(&obj))
        return;
    v8::Local<v8::Array> names;
    if (!obj->GetOwnPropertyNames(isolate->GetCurrentContext()).ToLocal(&names))
        return;
    info.GetReturnValue().Set(names);
}
