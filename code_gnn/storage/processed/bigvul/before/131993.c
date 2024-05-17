static void voidMethodVariadicTestInterfaceWillBeGarbageCollectedArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    WillBeHeapVector<RefPtrWillBeMember<TestInterfaceWillBeGarbageCollected> > variadicTestInterfaceWillBeGarbageCollectedArg;
    for (int i = 0; i < info.Length(); ++i) {
        if (!V8TestInterfaceWillBeGarbageCollected::hasInstance(info[i], info.GetIsolate())) {
            throwTypeError(ExceptionMessages::failedToExecute("voidMethodVariadicTestInterfaceWillBeGarbageCollectedArg", "TestObjectPython", "parameter 1 is not of type 'TestInterfaceWillBeGarbageCollected'."), info.GetIsolate());
            return;
        }
        variadicTestInterfaceWillBeGarbageCollectedArg.append(V8TestInterfaceWillBeGarbageCollected::toNative(v8::Handle<v8::Object>::Cast(info[i])));
    }
    imp->voidMethodVariadicTestInterfaceWillBeGarbageCollectedArg(variadicTestInterfaceWillBeGarbageCollectedArg);
}
