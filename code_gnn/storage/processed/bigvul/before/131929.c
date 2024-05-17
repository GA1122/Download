static void voidMethodNodeFilterArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodNodeFilterArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(RefPtr<NodeFilter>, nodeFilterArg, toNodeFilter(info[0], info.GetIsolate()));
    imp->voidMethodNodeFilterArg(nodeFilterArg.release());
}
