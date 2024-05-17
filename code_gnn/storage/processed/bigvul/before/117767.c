void V8TestNamedConstructor::derefObject(void* object)
{
    static_cast<TestNamedConstructor*>(object)->deref();
}
