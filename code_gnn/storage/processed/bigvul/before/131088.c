EventTarget* V8TestObject::toEventTarget(v8::Handle<v8::Object> object)
{
    return toNative(object);
}
