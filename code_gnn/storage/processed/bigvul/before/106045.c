JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, TestMediaQueryListListener* impl)
{
    return wrap<JSTestMediaQueryListListener>(exec, globalObject, impl);
}
