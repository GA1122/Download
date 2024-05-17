static void overloadedMethodA2Method(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 2)) {
        throwTypeError(ExceptionMessages::failedToExecute("overloadedMethodA", "TestObject", ExceptionMessages::notEnoughArguments(2, info.Length())), info.GetIsolate());
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(TestObject*, objArg, V8TestObject::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, strArg, info[1]);
    imp->overloadedMethodA(objArg, strArg);
}
