static void methodWithCallbackInterfaceAndOptionalArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    OwnPtr<TestCallbackInterface> callbackInterface;
    if (info.Length() > 0 && !isUndefinedOrNull(info[0])) {
        if (!info[0]->IsFunction()) {
            throwTypeError(ExceptionMessages::failedToExecute("methodWithCallbackInterfaceAndOptionalArg", "TestObject", "The callback provided as parameter 1 is not a function."), info.GetIsolate());
            return;
        }
        callbackInterface = V8TestCallbackInterface::create(v8::Handle<v8::Function>::Cast(info[0]), currentExecutionContext(info.GetIsolate()));
    }
    imp->methodWithCallbackInterfaceAndOptionalArg(callbackInterface.release());
}
