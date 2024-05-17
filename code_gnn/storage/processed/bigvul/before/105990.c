TestEventTarget* toTestEventTarget(JSC::JSValue value)
{
    return value.inherits(&JSTestEventTarget::s_info) ? jsCast<JSTestEventTarget*>(asObject(value))->impl() : 0;
}
