JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, TestNamedConstructor* impl)
{
    return wrap<JSTestNamedConstructor>(exec, globalObject, impl);
}
