static void methodWithOptionalArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "methodWithOptionalArg", "TestObject", info.Holder(), info.GetIsolate());
    TestObject* imp = V8TestObject::toNative(info.Holder());
    if (UNLIKELY(info.Length() <= 0)) {
        imp->methodWithOptionalArg();
        return;
    }
    V8TRYCATCH_EXCEPTION_VOID(int, opt, toInt32(info[0], exceptionState), exceptionState);
    imp->methodWithOptionalArg(opt);
}
