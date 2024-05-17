JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, TestSerializedScriptValueInterface* impl)
{
    return wrap<JSTestSerializedScriptValueInterface>(exec, globalObject, impl);
}
