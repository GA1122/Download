void setJSTestObjConditionalAttr4Constructor(ExecState* exec, JSObject* thisObject, JSValue value)
{
    jsCast<JSTestObj*>(thisObject)->putDirect(exec->globalData(), Identifier(exec, "conditionalAttr4"), value);
}
