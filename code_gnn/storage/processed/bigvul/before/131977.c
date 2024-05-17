static void voidMethodTestInterfaceEmptyArgVariadicTestInterfaceEmptyArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodTestInterfaceEmptyArgVariadicTestInterfaceEmptyArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(TestInterfaceEmpty*, testInterfaceEmptyArg, V8TestInterfaceEmpty::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    Vector<RefPtr<TestInterfaceEmpty> > variadicTestInterfaceEmptyArgs;
    for (int i = 1; i < info.Length(); ++i) {
        if (!V8TestInterfaceEmpty::hasInstance(info[i], info.GetIsolate())) {
            throwTypeError(ExceptionMessages::failedToExecute("voidMethodTestInterfaceEmptyArgVariadicTestInterfaceEmptyArg", "TestObjectPython", "parameter 2 is not of type 'TestInterfaceEmpty'."), info.GetIsolate());
            return;
        }
        variadicTestInterfaceEmptyArgs.append(V8TestInterfaceEmpty::toNative(v8::Handle<v8::Object>::Cast(info[i])));
    }
    imp->voidMethodTestInterfaceEmptyArgVariadicTestInterfaceEmptyArg(testInterfaceEmptyArg, variadicTestInterfaceEmptyArgs);
}
