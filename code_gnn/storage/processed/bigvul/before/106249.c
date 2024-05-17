JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, TestObj* impl)
{
    return wrap<JSTestObj>(exec, globalObject, impl);
}
