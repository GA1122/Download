static void DOMExceptionStackSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    ASSERT(info.Data()->IsObject());
    info.Data()->ToObject()->Set(v8String("stack", info.GetIsolate()), value);
}
