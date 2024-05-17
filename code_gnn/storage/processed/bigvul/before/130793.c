static void initializeScriptWrappableForInterface(TestObject* object)
{
    if (ScriptWrappable::wrapperCanBeStoredInObject(object))
        ScriptWrappable::setTypeInfoInObject(object, &V8TestObject::wrapperTypeInfo);
    else
        ASSERT_NOT_REACHED();
}
