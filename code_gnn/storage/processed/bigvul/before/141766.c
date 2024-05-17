void V8InjectedScriptHost::proxyTargetValueCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.Length() != 1 || !info[0]->IsProxy()) {
        NOTREACHED();
        return;
    }
    v8::Local<v8::Object> target = info[0].As<v8::Proxy>();
    while (target->IsProxy())
        target = v8::Local<v8::Proxy>::Cast(target)->GetTarget();
    info.GetReturnValue().Set(target);
}
