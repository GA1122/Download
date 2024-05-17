EncodedJSValue JSC_HOST_CALL JSDataViewConstructor::constructJSDataView(ExecState* exec)
{
    if (exec->argument(0).isNull() || !exec->argument(0).isObject())
        return throwVMTypeError(exec);

    RefPtr<DataView> view = constructArrayBufferViewWithArrayBufferArgument<DataView, char>(exec);
    if (!view.get()) {
        setDOMException(exec, INDEX_SIZE_ERR);
        return JSValue::encode(jsUndefined());
    }

    JSDataViewConstructor* jsConstructor = jsCast<JSDataViewConstructor*>(exec->callee());
    return JSValue::encode(asObject(toJS(exec, jsConstructor->globalObject(), view.get())));
}
