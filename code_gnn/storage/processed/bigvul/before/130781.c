static void eventHandlerAttrAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    EventListener* jsValue = imp->eventHandlerAttr();
    v8SetReturnValue(info, jsValue ? v8::Handle<v8::Value>(V8AbstractEventListener::cast(jsValue)->getListenerObject(imp->executionContext())) : v8::Handle<v8::Value>(v8::Null(info.GetIsolate())));
}
