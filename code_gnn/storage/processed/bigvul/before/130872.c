static void methodWithNonCallbackArgAndCallbackInterfaceArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "methodWithNonCallbackArgAndCallbackInterfaceArg", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 2)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(2, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(int, nonCallback, toInt32(info[0], exceptionState), exceptionState);
    if (info.Length() <= 1 || !info[1]->IsFunction()) {
        exceptionState.throwTypeError("The callback provided as parameter 2 is not a function.");
        exceptionState.throwIfNeeded();
        return;
    }
    OwnPtr<TestCallbackInterface> callbackInterface = V8TestCallbackInterface::create(v8::Handle<v8::Function>::Cast(info[1]), currentExecutionContext(info.GetIsolate()));
    imp->methodWithNonCallbackArgAndCallbackInterfaceArg(nonCallback, callbackInterface.release());
}
