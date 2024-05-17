JSValue toJS(ExecState* exec, JSDOMGlobalObject* globalObject, DataView* object)
{
    return wrap<JSDataView>(exec, globalObject, object);
}
