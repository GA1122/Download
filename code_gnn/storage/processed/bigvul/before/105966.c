TestCustomNamedGetter* toTestCustomNamedGetter(JSC::JSValue value)
{
    return value.inherits(&JSTestCustomNamedGetter::s_info) ? jsCast<JSTestCustomNamedGetter*>(asObject(value))->impl() : 0;
}
