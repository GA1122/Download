void setJSTestObjCustomAttr(ExecState* exec, JSObject* thisObject, JSValue value)
{
    jsCast<JSTestObj*>(thisObject)->setCustomAttr(exec, value);
}
