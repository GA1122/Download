void V8TestInterface::derefObject(void* object)
{
    static_cast<TestInterface*>(object)->deref();
}
