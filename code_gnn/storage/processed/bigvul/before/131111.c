static void variadicDoubleMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("variadicDoubleMethod", "TestObject", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(double, head, static_cast<double>(info[0]->NumberValue()));
    V8TRYCATCH_VOID(Vector<double>, tail, toNativeArguments<double>(info, 1));
    imp->variadicDoubleMethod(head, tail);
}
