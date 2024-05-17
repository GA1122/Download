TestNamedConstructor* toTestNamedConstructor(JSC::JSValue value)
{
    return value.inherits(&JSTestNamedConstructor::s_info) ? jsCast<JSTestNamedConstructor*>(asObject(value))->impl() : 0;
}
