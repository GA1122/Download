JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, TestInterface* impl)
{
    return wrap<JSTestInterface>(exec, globalObject, impl);
}
