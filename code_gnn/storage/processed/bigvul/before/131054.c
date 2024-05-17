static void staticMethodWithCallbackArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("staticMethodWithCallbackArg", "TestObject", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    if (info.Length() <= 0 || !info[0]->IsFunction()) {
        throwTypeError(ExceptionMessages::failedToExecute("staticMethodWithCallbackArg", "TestObject", "The callback provided as parameter 1 is not a function."), info.GetIsolate());
        return;
    }
    OwnPtr<TestCallbackInterface> callbackInterface = V8TestCallbackInterface::create(v8::Handle<v8::Function>::Cast(info[0]), currentExecutionContext(info.GetIsolate()));
    TestObject::staticMethodWithCallbackArg(callbackInterface.release());
}
