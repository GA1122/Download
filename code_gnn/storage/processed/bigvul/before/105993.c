JSTestInterface::JSTestInterface(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<TestInterface> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}
