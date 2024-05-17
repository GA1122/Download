void V8Console::CommandLineAPIScope::accessorGetterCallback(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CommandLineAPIScope* scope = static_cast<CommandLineAPIScope*>(info.Data().As<v8::External>()->Value());
    DCHECK(scope);

    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
    if (scope->m_cleanup) {
        bool removed = info.Holder()->Delete(context, name).FromMaybe(false);
        DCHECK(removed);
        return;
    }
    v8::Local<v8::Object> commandLineAPI = scope->m_commandLineAPI;

    v8::Local<v8::Value> value;
    if (!commandLineAPI->Get(context, name).ToLocal(&value))
        return;
    if (isCommandLineAPIGetter(toProtocolStringWithTypeCheck(name))) {
        DCHECK(value->IsFunction());
        v8::MicrotasksScope microtasks(info.GetIsolate(), v8::MicrotasksScope::kDoNotRunMicrotasks);
        if (value.As<v8::Function>()->Call(context, commandLineAPI, 0, nullptr).ToLocal(&value))
            info.GetReturnValue().Set(value);
    } else {
        info.GetReturnValue().Set(value);
    }
}
