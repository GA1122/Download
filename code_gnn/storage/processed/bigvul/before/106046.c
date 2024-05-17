TestMediaQueryListListener* toTestMediaQueryListListener(JSC::JSValue value)
{
    return value.inherits(&JSTestMediaQueryListListener::s_info) ? jsCast<JSTestMediaQueryListListener*>(asObject(value))->impl() : 0;
}
