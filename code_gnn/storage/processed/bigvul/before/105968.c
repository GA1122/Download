JSTestEventTarget::JSTestEventTarget(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<TestEventTarget> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}
