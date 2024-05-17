static void eventHandlerAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    EventListener* jsValue = imp->eventHandlerAttribute();
    v8SetReturnValue(info, jsValue ? v8::Handle<v8::Value>(V8AbstractEventListener::cast(jsValue)->getListenerObject(imp->executionContext())) : v8::Handle<v8::Value>(v8::Null(info.GetIsolate())));
}
