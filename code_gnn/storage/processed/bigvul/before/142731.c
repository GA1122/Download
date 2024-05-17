void invokeOnScriptableObject(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    HTMLPlugInElement* impl = ElementType::toImpl(info.Holder());
    RefPtr<SharedPersistent<v8::Object>> wrapper = impl->pluginWrapper();
    if (!wrapper)
        return;

    v8::Local<v8::Object> instance = wrapper->newLocal(info.GetIsolate());
    if (instance.IsEmpty())
        return;

    std::unique_ptr<v8::Local<v8::Value>[] > arguments = wrapArrayUnique(new v8::Local<v8::Value>[info.Length()]);
    for (int i = 0; i < info.Length(); ++i)
        arguments[i] = info[i];

    v8::Local<v8::Value> retVal;
    if (!instance->CallAsFunction(info.GetIsolate()->GetCurrentContext(), info.Holder(), info.Length(), arguments.get()).ToLocal(&retVal))
        return;
    v8SetReturnValue(info, retVal);
}
