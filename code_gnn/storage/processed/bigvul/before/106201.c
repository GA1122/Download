void setJSTestObjConditionalAttr5Constructor(ExecState* exec, JSObject* thisObject, JSValue value)
{
    jsCast<JSTestObj*>(thisObject)->putDirect(exec->globalData(), Identifier(exec, "conditionalAttr5"), value);
}
