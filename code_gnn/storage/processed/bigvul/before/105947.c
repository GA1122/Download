JSTestCustomNamedGetter::JSTestCustomNamedGetter(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<TestCustomNamedGetter> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}
