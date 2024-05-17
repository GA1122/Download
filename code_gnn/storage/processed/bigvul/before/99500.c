static bool NPN_InvokeDefault(NPP, NPObject *npObject, const NPVariant* arguments, uint32_t argumentCount, NPVariant* result)
{
    if (npObject->_class->invokeDefault)
        return npObject->_class->invokeDefault(npObject, arguments, argumentCount, result);

    return false;
}
