static void callWithActiveWindowMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    imp->callWithActiveWindow(callingDOMWindow(info.GetIsolate()));
}
