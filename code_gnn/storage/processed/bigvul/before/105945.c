TestActiveDOMObject* toTestActiveDOMObject(JSC::JSValue value)
{
    return value.inherits(&JSTestActiveDOMObject::s_info) ? jsCast<JSTestActiveDOMObject*>(asObject(value))->impl() : 0;
}
