static void voidMethodWithArgsMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "voidMethodWithArgs", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 3)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(3, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(int, longArg, toInt32(info[0], exceptionState), exceptionState);
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, strArg, info[1]);
    V8TRYCATCH_VOID(TestObject*, objArg, V8TestObject::toNativeWithTypeCheck(info.GetIsolate(), info[2]));
    imp->voidMethodWithArgs(longArg, strArg, objArg);
}
