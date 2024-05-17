static void voidMethodOptionalTestInterfaceEmptyArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    if (UNLIKELY(info.Length() <= 0)) {
        imp->voidMethodOptionalTestInterfaceEmptyArg();
        return;
    }
    V8TRYCATCH_VOID(TestInterfaceEmpty*, optionalTestInterfaceEmptyArg, V8TestInterfaceEmpty::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    imp->voidMethodOptionalTestInterfaceEmptyArg(optionalTestInterfaceEmptyArg);
}
