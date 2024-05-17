void V8TestCustomNamedGetter::derefObject(void* object)
{
    static_cast<TestCustomNamedGetter*>(object)->deref();
}
