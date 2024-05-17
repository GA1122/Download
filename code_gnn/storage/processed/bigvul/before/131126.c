static void withActiveWindowAndFirstWindowMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    imp->withActiveWindowAndFirstWindow(callingDOMWindow(info.GetIsolate()), enteredDOMWindow(info.GetIsolate()));
}
