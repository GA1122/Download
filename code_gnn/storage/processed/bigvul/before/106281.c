TestSerializedScriptValueInterface* toTestSerializedScriptValueInterface(JSC::JSValue value)
{
    return value.inherits(&JSTestSerializedScriptValueInterface::s_info) ? jsCast<JSTestSerializedScriptValueInterface*>(asObject(value))->impl() : 0;
}
