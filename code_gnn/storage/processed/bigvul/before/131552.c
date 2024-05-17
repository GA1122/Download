static void overloadedMethodH2Method(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("overloadedMethodH", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    if (info.Length() <= 0 || !info[0]->IsFunction()) {
        throwTypeError(ExceptionMessages::failedToExecute("overloadedMethodH", "TestObjectPython", "The callback provided as parameter 1 is not a function."), info.GetIsolate());
        return;
    }
    OwnPtr<TestCallbackInterface> testCallbackInterfaceArg = V8TestCallbackInterface::create(v8::Handle<v8::Function>::Cast(info[0]), currentExecutionContext(info.GetIsolate()));
    imp->overloadedMethodH(testCallbackInterfaceArg.release());
}
