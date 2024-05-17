static void methodWithOptionalStringMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    if (UNLIKELY(info.Length() <= 0)) {
        imp->methodWithOptionalString();
        return;
    }
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, str, info[0]);
    imp->methodWithOptionalString(str);
}
