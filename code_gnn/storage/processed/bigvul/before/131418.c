static void initializeScriptWrappableForInterface(TestObjectPython* object)
{
    if (ScriptWrappable::wrapperCanBeStoredInObject(object))
        ScriptWrappable::setTypeInfoInObject(object, &V8TestObjectPython::wrapperTypeInfo);
    else
        ASSERT_NOT_REACHED();
}
