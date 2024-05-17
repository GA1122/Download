TestObj* toTestObj(JSC::JSValue value)
{
    return value.inherits(&JSTestObj::s_info) ? jsCast<JSTestObj*>(asObject(value))->impl() : 0;
}
