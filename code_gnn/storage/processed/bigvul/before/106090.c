JSValue jsTestObjCONST_JAVASCRIPT(ExecState* exec, JSValue, const Identifier&)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(15));
}
