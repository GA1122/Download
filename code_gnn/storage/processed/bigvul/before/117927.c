void V8TestSerializedScriptValueInterface::derefObject(void* object)
{
    static_cast<TestSerializedScriptValueInterface*>(object)->deref();
}
