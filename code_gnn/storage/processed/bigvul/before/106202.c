void setJSTestObjConditionalAttr6Constructor(ExecState* exec, JSObject* thisObject, JSValue value)
{
    jsCast<JSTestObj*>(thisObject)->putDirect(exec->globalData(), Identifier(exec, "conditionalAttr6"), value);
}
