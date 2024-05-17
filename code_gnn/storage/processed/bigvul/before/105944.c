JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, TestActiveDOMObject* impl)
{
    return wrap<JSTestActiveDOMObject>(exec, globalObject, impl);
}
