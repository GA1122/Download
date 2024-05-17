void V8TestMediaQueryListListener::derefObject(void* object)
{
    static_cast<TestMediaQueryListListener*>(object)->deref();
}
