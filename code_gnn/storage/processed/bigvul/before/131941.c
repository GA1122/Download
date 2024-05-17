static void voidMethodOptionalStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    if (UNLIKELY(info.Length() <= 0)) {
        imp->voidMethodOptionalStringArg();
        return;
    }
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, optionalStringArg, info[0]);
    imp->voidMethodOptionalStringArg(optionalStringArg);
}
