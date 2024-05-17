static void eventHandlerAttrAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    moveEventListenerToNewWrapper(info.Holder(), imp->eventHandlerAttr(), jsValue, V8TestObject::eventListenerCacheIndex, info.GetIsolate());
    imp->setEventHandlerAttr(V8EventListenerList::getEventListener(jsValue, true, ListenerFindOrCreate));
}
