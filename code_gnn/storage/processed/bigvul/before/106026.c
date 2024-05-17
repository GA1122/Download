TestInterface* toTestInterface(JSC::JSValue value)
{
    return value.inherits(&JSTestInterface::s_info) ? jsCast<JSTestInterface*>(asObject(value))->impl() : 0;
}
