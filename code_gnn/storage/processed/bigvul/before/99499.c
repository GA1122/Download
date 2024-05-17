static bool NPN_Invoke(NPP, NPObject *npObject, NPIdentifier methodName, const NPVariant* arguments, uint32_t argumentCount, NPVariant* result)
{
    if (npObject->_class->invoke)
        return npObject->_class->invoke(npObject, methodName, arguments, argumentCount, result);

    return false;
}
