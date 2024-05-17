JSTestMediaQueryListListener::JSTestMediaQueryListListener(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<TestMediaQueryListListener> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}
