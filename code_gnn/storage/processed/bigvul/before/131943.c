static void voidMethodOptionalTestCallbackInterfaceArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    OwnPtr<TestCallbackInterface> optionalTestCallbackInterfaceArg;
    if (info.Length() > 0 && !isUndefinedOrNull(info[0])) {
        if (!info[0]->IsFunction()) {
            throwTypeError(ExceptionMessages::failedToExecute("voidMethodOptionalTestCallbackInterfaceArg", "TestObjectPython", "The callback provided as parameter 1 is not a function."), info.GetIsolate());
            return;
        }
        optionalTestCallbackInterfaceArg = V8TestCallbackInterface::create(v8::Handle<v8::Function>::Cast(info[0]), currentExecutionContext(info.GetIsolate()));
    }
    imp->voidMethodOptionalTestCallbackInterfaceArg(optionalTestCallbackInterfaceArg.release());
}
