static void voidMethodVariadicTestInterfaceEmptyArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    Vector<RefPtr<TestInterfaceEmpty> > variadicTestInterfaceEmptyArgs;
    for (int i = 0; i < info.Length(); ++i) {
        if (!V8TestInterfaceEmpty::hasInstance(info[i], info.GetIsolate())) {
            throwTypeError(ExceptionMessages::failedToExecute("voidMethodVariadicTestInterfaceEmptyArg", "TestObjectPython", "parameter 1 is not of type 'TestInterfaceEmpty'."), info.GetIsolate());
            return;
        }
        variadicTestInterfaceEmptyArgs.append(V8TestInterfaceEmpty::toNative(v8::Handle<v8::Object>::Cast(info[i])));
    }
    imp->voidMethodVariadicTestInterfaceEmptyArg(variadicTestInterfaceEmptyArgs);
}
