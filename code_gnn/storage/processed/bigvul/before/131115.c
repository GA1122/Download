static void variadicStringMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("variadicStringMethod", "TestObject", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, head, info[0]);
    V8TRYCATCH_VOID(Vector<String>, tail, toNativeArguments<String>(info, 1));
    imp->variadicStringMethod(head, tail);
}
