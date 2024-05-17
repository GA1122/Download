static void overloadedMethodE2Method(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("overloadedMethodE", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Vector<int>, longArrayArg, toNativeArray<int>(info[0], 1, info.GetIsolate()));
    imp->overloadedMethodE(longArrayArg);
}
