JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, TestCustomNamedGetter* impl)
{
    return wrap<JSTestCustomNamedGetter>(exec, globalObject, impl);
}
