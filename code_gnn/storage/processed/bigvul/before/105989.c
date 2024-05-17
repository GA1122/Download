JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, TestEventTarget* impl)
{
    return wrap<JSTestEventTarget>(exec, globalObject, impl);
}
