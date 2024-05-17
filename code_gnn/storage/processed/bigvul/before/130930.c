static void overloadedMethod3Method(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("overloadedMethod", "TestObject", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(TestObject*, objArg, V8TestObject::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    imp->overloadedMethod(objArg);
}
