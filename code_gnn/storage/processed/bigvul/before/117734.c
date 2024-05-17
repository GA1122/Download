void V8TestEventConstructor::derefObject(void* object)
{
    static_cast<TestEventConstructor*>(object)->deref();
}
