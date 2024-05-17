void setJSTestInterfaceSupplementalStr3(ExecState* exec, JSObject* thisObject, JSValue value)
{
    jsCast<JSTestInterface*>(thisObject)->setSupplementalStr3(exec, value);
}
