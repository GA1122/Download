static void eventHandlerAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    moveEventListenerToNewWrapper(info.Holder(), imp->eventHandlerAttribute(), jsValue, V8TestObjectPython::eventListenerCacheIndex, info.GetIsolate());
    imp->setEventHandlerAttribute(V8EventListenerList::getEventListener(jsValue, true, ListenerFindOrCreate));
}
