void V8Console::CommandLineAPIScope::accessorSetterCallback(v8::Local<v8::Name> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CommandLineAPIScope* scope = static_cast<CommandLineAPIScope*>(info.Data().As<v8::External>()->Value());
    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
    if (!info.Holder()->Delete(context, name).FromMaybe(false))
        return;
    if (!info.Holder()->CreateDataProperty(context, name, value).FromMaybe(false))
        return;
    bool removed = scope->m_installedMethods->Delete(context, name).FromMaybe(false);
    DCHECK(removed);
}
