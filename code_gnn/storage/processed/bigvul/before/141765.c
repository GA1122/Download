void V8InjectedScriptHost::objectHasOwnPropertyCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.Length() < 2 || !info[0]->IsObject() || !info[1]->IsString())
        return;
    bool result = info[0].As<v8::Object>()->HasOwnProperty(info.GetIsolate()->GetCurrentContext(), v8::Local<v8::String>::Cast(info[1])).FromMaybe(false);
    info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), result));
}
